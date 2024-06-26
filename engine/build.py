import os
import sys
import platform
from argparse import ArgumentParser

parser = ArgumentParser(description="CLI")
parser.add_argument(
    "-g",
    action="store_true",
    help="Whether the GCC should store debugging data for GDB",
)
args = parser.parse_args()

MINGW_PATH = r"C:\msys64\mingw64"
os_name = "win32" if platform.system() == "Windows" else "linux"

files = []

script_dir = os.path.dirname(os.path.realpath(sys.argv[0]))


def run_cmd(s):
    print("> " + s + "\n")
    if os.system(s) != 0:
        exit(1)


includeW = (
    f' -I"{os.path.normpath(f"{MINGW_PATH}/include")}"' if os_name == "win32" else ""
)
engineInclude = os.path.normpath(f"{script_dir}/include")

if not os.path.exists(f"{script_dir}/lib/{os_name}"):
    os.makedirs(f"{script_dir}/lib/{os_name}")

for entry in os.listdir(f"{script_dir}/src"):
    name = os.path.splitext(entry)[0]
    files.append(name)
    cFile = os.path.normpath(f"{script_dir}/src/{name}.c")
    oFile = os.path.normpath(f"{script_dir}/lib/{os_name}/{name}.o")
    run_cmd(
        f'gcc {"-g " if args.g else ""}-I"{engineInclude}"{includeW} -c "{cFile}" -o "{oFile}" -lSDL2 -lSDL2_image'
    )

oFiles = " ".join(
    map(lambda f: os.path.normpath(f'"{script_dir}/lib/{os_name}/{f}.o"'), files)
)
aFile = os.path.normpath(f"{script_dir}/lib/{os_name}/libhelium.a")
run_cmd(f'ar rcs "{aFile}" {oFiles}')

for f in files:
    os.remove(os.path.normpath(f"{script_dir}/lib/{os_name}/{f}.o"))
