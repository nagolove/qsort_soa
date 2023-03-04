workspace "CRAZY_ALGOS"
    configurations { "ONE" }

project "qsort"
    targetdir "."
    files { "qsort.c" }
    kind "ConsoleApp"
    --filter "configurations:ONE"
        --symbols "On"


