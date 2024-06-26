import os
import platform
import sys
import shutil
from os import path
from pathlib import Path
from argparse import ArgumentParser

parser = ArgumentParser(description="CLI")
parser.add_argument(
    "directory",
    help="Directory of the game project",
)
parser.add_argument(
    "-g",
    action="store_true",
    help="Whether the GCC should store debugging data for GDB",
)
parser.add_argument(
    "-w",
    action="store_true",
    help="On windows, if this option is enabled the exe file won't open a command prompt when executed",
)
args = parser.parse_args()


def run_cmd(s):
    print(f"> {s}\n\n")
    if os.system(s) != 0:
        exit(1)


def scan_files(directory):
    directory = Path(directory)
    return [str(p) for p in directory.glob("**/*") if p.is_file()]


MINGW_PATH = r"C:\msys64\mingw64"

os_name = "win32" if platform.system() == "Windows" else "linux"

script_dir = path.dirname(path.realpath(sys.argv[0]))

target_dir = sys.argv[1] if len(sys.argv) > 1 else os.getcwd()

target_name = os.path.basename(target_dir)

target_bin = path.normpath(f"{target_dir}/bin/{os_name}")
target_exe = path.normpath(
    target_bin
    + "/"
    + (f"{target_name}.exe" if os_name == "win32" else f"{target_name}")
)

if path.exists(target_bin):
    shutil.rmtree(target_bin)

os.makedirs(target_bin)

includes = []
libs = []
flags = []

includes.append(path.join(script_dir, "include"))
libs.append(path.join(script_dir, "lib"))
flags.append("-lhelium")
flags.append("-lSDL2main")
flags.append("-lSDL2")
flags.append("-lSDL2_image")
flags.append("-lm")

if os_name == "win32":
    # This is for SDL
    includes.append(path.join(MINGW_PATH, "include"))
    libs.append(path.join(MINGW_PATH, "lib"))
    if args.w:
        flags.append("-mwindows")
    flags.append("-DSDL_MAIN_HANDLED")

if args.g:
    flags.append("-g")

cFiles = "".join(
    map(lambda x: f'"{x}" ', filter(lambda x: x.endswith(".c"), scan_files(target_dir)))
)
includeStr = "".join(map(lambda l: f'-I"{l}" ', includes))
libStr = "".join(map(lambda l: f'-L"{l}" ', libs))
flagStr = " ".join(flags)

run_cmd(f'gcc -o "{target_exe}" {cFiles}{includeStr}{libStr}{flagStr}')

if os_name == "win32":
    shutil.copy(path.join(script_dir, "SDL2.dll"), path.join(target_bin, "SDL2.dll"))
    shutil.copy(
        path.join(script_dir, "SDL2_image.dll"), path.join(target_bin, "SDL2_image.dll")
    )

shutil.copytree(
    path.join(target_dir, "assets"), path.join(target_bin, "assets"), dirs_exist_ok=True
)
