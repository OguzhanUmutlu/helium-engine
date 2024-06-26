import os
import sys
import platform
import shutil
from zipfile import ZipFile, ZIP_DEFLATED
from argparse import ArgumentParser

parser = ArgumentParser(description="CLI")
parser.add_argument(
    "-g",
    action="store_true",
    help="Whether the GCC should store debugging data for GDB",
)
args = parser.parse_args()

version = "1.0.1"

script_dir = os.path.dirname(os.path.realpath(sys.argv[0]))
os_name = "win32" if platform.system() == "Windows" else "linux"


def run_cmd(s):
    print("> " + s + "\n")
    if os.system(s) != 0:
        exit(1)


"""run_cmd(
    f'"{script_dir}\\..\\cli\\build.cmd"'
    if os_name == "win32"
    else f"{script_dir}/../cli/build.sh"
)"""

sdlPy = os.path.normpath(f"{script_dir}/sdl.py")
run_cmd(f"python3 {sdlPy}")

buildPy = os.path.normpath(f"{script_dir}/../engine/build.py")
gFlag = " -g" if args.g else ""
run_cmd(f"python3 {buildPy}{gFlag}")

dir = f"{script_dir}/../releases/{version}/"
dw = dir + "win32/"
dl = dir + "linux/"

os.makedirs(dw + "lib", exist_ok=True)
os.makedirs(dl + "lib", exist_ok=True)

shutil.copytree(f"{script_dir}/../engine/include", dw + "include", dirs_exist_ok=True)
shutil.copytree(f"{script_dir}/../engine/include", dl + "include", dirs_exist_ok=True)
# shutil.copy(f"{script_dir}/../cli/bin/win32/cli.exe", dw + "helium.exe")
# shutil.copy(f"{script_dir}/../cli/bin/linux/cli", dl + "helium")
shutil.copy(f"{script_dir}/../cli/cli.py", dw + "helium.py")
shutil.copy(f"{script_dir}/../cli/cli.py", dl + "helium.py")
# shutil.copy(f"{script_dir}/sdl.py", dw + "sdl.py")
# shutil.copy(f"{script_dir}/update_sdl.cmd", dw + "update_sdl.cmd")
shutil.copy(f"{script_dir}/../engine/lib/win32/libhelium.a", dw + "lib/libhelium.a")
shutil.copy(f"{script_dir}/../engine/lib/linux/libhelium.a", dl + "lib/libhelium.a")
shutil.copy(f"{script_dir}/SDL2.dll", f"{dw}/SDL2.dll")
shutil.copy(f"{script_dir}/SDL2_image.dll", f"{dw}/SDL2_image.dll")



def zip_folder(d, dest):
    with ZipFile(dest, "w", ZIP_DEFLATED) as zip:
        for root, _, files in os.walk(d):
            for file in files:
                file_path = os.path.join(root, file)
                zip.write(file_path, os.path.relpath(file_path, d))


zip_folder(dw, f"{dir}Helium-{version}-win32.zip")
zip_folder(dl, f"{dir}Helium-{version}-linux.zip")

dlat = f"{script_dir}/../releases/latest/"
dlat_dev = f"{script_dir}/../releases/latest-dev/"
if os.path.exists(dlat):
    shutil.rmtree(dlat)
if os.path.exists(dlat_dev):
    shutil.rmtree(dlat_dev)
shutil.copytree(f"{script_dir}/../releases/{version}/", dlat)
shutil.copytree(f"{script_dir}/../releases/{version}/", dlat_dev)

shutil.rmtree(dw)
shutil.rmtree(dl)
shutil.rmtree(f"{dlat}/win32")
shutil.rmtree(f"{dlat}/linux")
os.remove(f"{dlat_dev}/Helium-{version}-win32.zip")
os.remove(f"{dlat_dev}/Helium-{version}-linux.zip")