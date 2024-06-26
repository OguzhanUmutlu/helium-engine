import os
import requests
from zipfile import ZipFile
import shutil


def download_sdl(url, dll):
    print("Fetching " + dll + "'s latest version...")
    latest = requests.get(url).json()

    download_url = ""

    for i in map(lambda x: x["browser_download_url"], latest.get("assets")):
        if i.endswith("win32-x64.zip"):
            download_url = i
            break

    print(dll + "'s latest version has been fetched: " + latest["name"])
    print(dll + "'s download url: " + download_url)
    print("Installing " + dll + "'s zip...")

    with requests.get(download_url, stream=True) as r:
        r.raise_for_status()
        with open("downloading_sdl.zip", "wb") as f:
            for chunk in r.iter_content(chunk_size=8192):
                f.write(chunk)

    print(dll + "'s zip has been installed")

    with ZipFile("downloading_sdl.zip", "r") as zip_ref:
        zip_ref.extractall("downloading_sdl")

    print(dll + "'s zip has been extracted")

    shutil.copy("downloading_sdl/" + dll, dll)
    os.remove("downloading_sdl.zip")
    shutil.rmtree("downloading_sdl")
    print(dll + " has been successfully installed")


SDL_API_URL = "https://api.github.com/repos/libsdl-org/SDL/releases/latest"
SDL_IMAGE_API_URL = "https://api.github.com/repos/libsdl-org/SDL_image/releases/latest"

download_sdl(SDL_API_URL, "SDL2.dll")
download_sdl(SDL_IMAGE_API_URL, "SDL2_image.dll")
