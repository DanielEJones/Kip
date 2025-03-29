from os import path, listdir
from subprocess import run

from Tamarin import run_tests


class CompileError(Exception):
    ...


def find_files_in(dir: str) -> list[str]:
    return [
        path.join(dir, file)
        for file in listdir(dir)
        if file.endswith(".c")
    ]

def prefix(pre: str):
    return lambda s: f'{pre}{s}'

def compile() -> None:
    res = run([
        "cc", "-Wall", "-Werror",
        "-o", "main",
        "main.c", *find_files_in("src/")
    ], capture_output = True)

    if res.returncode:
        raise CompileError(
            '\n'.join([
                "[Error] compilation failed:",
                *map(prefix("  | "), res.stderr.decode().splitlines())
            ])
        )

def kip_exec(command: list[str] | str, stderr: bool = False) -> bytes:
    command = [command] if not isinstance(command, list) else command
    res = run(["./main", *command], capture_output=True)
    return res.stdout if not stderr else res.stderr

def main():
    try:
        compile()
        run_tests()
    except CompileError as e:
        print(e)

