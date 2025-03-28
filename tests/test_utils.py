from os import path, listdir
from subprocess import run


class CompileError(Exception):
    ...


def find_files(dir: str) -> list[str]:
    return [
        path.join(file)
        for file in listdir(dir)
        if file.endswith(".c")
    ]

def prefix(pre: str):
    return lambda s: f'{pre}{s}'

def compile() -> None:
    res = run([
        "cc", "-Wall", "-Werror",
        "-o", "main",
        "main.c", *find_files("src")
    ], capture_output = True)

    if res.returncode:
        raise CompileError(
            '\n'.join([
                "[Error] compilation failed:",
                *map(prefix("  | "), res.stderr.decode().splitlines())
            ])
        )

