current_dir=$(pwd)
cd "$(dirname "$0")"

if [ ! -d "bin/linux" ]; then
    mkdir -p "bin/linux"
fi

pip install pyinstaller
pyinstaller --onefile cli.py --distpath=bin/linux --name=cli

rm cli.spec

cd "$current_dir"