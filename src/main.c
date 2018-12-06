#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set_modes.h"
#include "setup.h"

void printusage(const char* exec) {
  printf("\n");
  printf("Usage: %s {custom | preset} [params...]\n", exec);
  printf("Control Fusion RGB Keyboard on Gigabyte Aero 15X\n");
  printf("\n");
  printf("  mode specific params:\n");
  printf("    custom    <config file>\n");
  printf("    preset    <name> <color> [speed (0-10)]\n");
  printf("\n");
  printf("  available colors: {red|green|yellow|blue|orange|purple|white}\n");
  printf("\n");
  printf("  preset names + params:\n");
  printf("    static             {color | cycle}  \n");
  printf("    breathing          {color | cycle}  \n");
  printf("    wave                                \n");
  printf("    fade_on_keypress   {color | rainbow}\n");
  printf("    marquee            {color | cycle}  \n");
  printf("    ripple             {color | rainbow}\n");
  printf("    flash_on_keypress  {color | rainbow}\n");
  printf("    neon                                \n");
  printf("    fill_marquee       {color | rainbow}\n");
  printf("    raindrop           {color | rainbow}\n");
  printf("    circle_marquee     {color | cycle}  \n");
  printf("    hedge              {color | cycle}  \n");
  printf("    rotate             {color | cycle}  \n");
  printf("\n");
  exit(EXIT_FAILURE);
}

struct Args {
  enum { M_PRESET, M_CUSTOM } mode;
  union {
    struct {
      uint8_t mode;
      uint8_t color;
      uint8_t speed;
    } preset;

    struct {
      const char* cfg_path;
    } custom;
  };
};

int main(int argc, char** argv) {
  if (argc == 1) printusage(argv[0]);

  // params
  struct Args args;

  // first level - mode selection
  const char* mode_s = argv[1];
  /**/ if (!strcmp(mode_s, "custom")) args.mode = M_CUSTOM;
  else if (!strcmp(mode_s, "preset")) args.mode = M_PRESET;
  else printusage(argv[0]);

  // second level - per-mode argument parsing
  if (args.mode == M_PRESET) {
    // get preset
    if (argc < 3) {
      fprintf(stderr, "No preset specified!\n");
      exit(EXIT_FAILURE);
    }

    static struct {
      const char* preset_s;
      const uint8_t mode;
    } preset_map [] = {
      { "static",            PRESET_STATIC            },
      { "breathing",         PRESET_BREATHING         },
      { "wave",              PRESET_WAVE              },
      { "fade_on_keypress",  PRESET_FADE_ON_KEYPRESS  },
      { "marquee",           PRESET_MARQUEE           },
      { "ripple",            PRESET_RIPPLE            },
      { "flash_on_keypress", PRESET_FLASH_ON_KEYPRESS },
      { "neon",              PRESET_NEON              },
      { "fill_marquee",      PRESET_RAINBOW_MARQUEE   },
      { "raindrop",          PRESET_RAINDROP          },
      { "circle_marquee",    PRESET_CIRCLE_MARQUEE    },
      { "hedge",             PRESET_HEDGE             },
      { "rotate",            PRESET_ROTATE            },
    };

    args.preset.mode = 0;
    for (uint i = 0; i < sizeof(preset_map)/sizeof(preset_map[0]); i++) {
      if (!strcmp(preset_map[i].preset_s, argv[2])) {
        args.preset.mode = preset_map[i].mode;
        break;
      }
    }

    if (!args.preset.mode) {
      fprintf(stderr, "invalid preset specified!\n");
      exit(EXIT_FAILURE);
    }

    const bool optional_color =
        args.preset.mode == PRESET_WAVE || args.preset.mode == PRESET_NEON;

    // get color
    if (!optional_color) {
      if (argc < 4) {
        fprintf(stderr, "No color specified!\n");
        exit(EXIT_FAILURE);
      }

      static struct {
        const char* color_s;
        const uint8_t color_val;
      } color_map [] = {
        { "rand",    COLOR_RAND   },
        { "cycle",   COLOR_RAND   },
        { "rainbow", COLOR_RAND   },
        { "red",     COLOR_RED    },
        { "green",   COLOR_GREEN  },
        { "yellow",  COLOR_YELLOW },
        { "blue",    COLOR_BLUE   },
        { "orange",  COLOR_ORANGE },
        { "purple",  COLOR_PURPLE },
        { "white",   COLOR_WHITE  },
      };

      args.preset.color = 0x08; // invalid color
      for (uint i = 0; i < sizeof(color_map)/sizeof(color_map[0]); i++) {
        if (!strcmp(color_map[i].color_s, argv[3])) {
          args.preset.color = color_map[i].color_val;
          break;
        }
      }

      if (args.preset.color == 0x08) {
        fprintf(stderr, "invalid color specified!\n");
        exit(EXIT_FAILURE);
      }
    }

    // BAD CODE: handle speed for wave and neon
    if (optional_color && argc == 4) {
      int speed = atoi(argv[3]);
      if (speed < 0 || speed > 10) {
        fprintf(stderr, "speed must be between 0 and 10!\n");
        exit(EXIT_FAILURE);
      }
      args.preset.speed = speed;
    }

    // potentially custom speed
    if (argc == 5) {
      int speed = atoi(argv[4]);
      if (speed < 0 || speed > 10) {
        fprintf(stderr, "speed must be between 0 and 10!\n");
        exit(EXIT_FAILURE);
      }
      args.preset.speed = speed;
    }

    // do some sanity-checks
    switch (args.preset.mode) {
    // neon won't work without COLOR_RAND
    case PRESET_NEON: args.preset.color = COLOR_RAND; break;
    // prevent seizures
    case PRESET_WAVE:   args.preset.speed = args.preset.speed ?: 1; break;
    case PRESET_STATIC: args.preset.speed = args.preset.speed ?: 1; break;
    }
  }

  if (args.mode == M_CUSTOM) {
    if (argc < 3) {
      fprintf(stderr, "No config file specified!\n");
      exit(EXIT_FAILURE);
    }
    args.custom.cfg_path = argv[2];
  }



  // set-up libusb
  libusb_context* ctx = NULL;
  libusb_device_handle* handle = NULL;

  int error_libusb = fusionkbd_libusb_setup(&ctx, &handle);
  if (error_libusb < 0) exit(EXIT_FAILURE);



  // execute specified commands
  int exitcode = 0;

  if (args.mode == M_PRESET) {
    bool err = set_mode(handle,
      args.preset.mode,
      args.preset.color,
      0x50 / 3,
      args.preset.speed);

    if (err) {
      fprintf(stderr, "Failed to set preset mode!\n");
      exitcode = 1;
      goto exit;
    }
  }

  if (args.mode == M_CUSTOM) {
    // Custom mode
    FILE* fd = fopen(args.custom.cfg_path, "rb");
    if (!fd) {
      fprintf(stderr, "fopen(%s) failed: %s\n", args.custom.cfg_path, strerror(errno));
      exitcode = 1;
      goto exit;
    }

    // see data.inc for data-format
    #include "data.inc"  // uint8_t m_blank_data[512];

    size_t lenread = fread(m_blank_data, 512, 1, fd);
    if (lenread != 1) {
      fprintf(stderr, "invalid cfg file!\n");
      exitcode = 1;
      goto exit;
    }

    fclose(fd);

    bool err = set_custom_mode(handle, m_blank_data, 0x50 / 3);
    if (err) {
      fprintf(stderr, "Failed to set custom mode!\n");
      exitcode = 1;
      goto exit;
    }
  }

exit:
  fusionkbd_libusb_cleanup(ctx, handle);
  return exitcode;
}
