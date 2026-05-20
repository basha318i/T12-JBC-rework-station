Import("env")

env.AddPostAction(
    "$BUILD_DIR/${PROGNAME}.elf",
    env.VerboseAction(
        " ".join([
            "arm-none-eabi-objcopy",
            "-O", "ihex",
            "$BUILD_DIR/${PROGNAME}.elf",
            "$BUILD_DIR/${PROGNAME}.hex"
        ]),
        "Generating HEX"
    )
)