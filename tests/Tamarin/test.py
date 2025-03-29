from __future__ import annotations
from typing import Any, Callable
from functools import wraps

from .exceptions import DuplicateMethodCall, TestFailed


TAMARIN_TEST_LIST = []


def run_tests():
    fails = 0
    for i, test in enumerate(TAMARIN_TEST_LIST):
        print(f"{i + 1}/{len(TAMARIN_TEST_LIST)}: ", end='')
        try:
            test()
            print("TEST OK.")
        except TestFailed as e:
            fails += 1
            print(e)
    print('-' * 75)
    print("OK." if fails == 0 else f"{fails} Fail(s).")

def chain_wrapper(method: Callable[..., None]) -> Callable[..., _Test]:
    @wraps(method)
    def inner(self: _Test, *args, **kwargs) -> _Test:
        if method in self._called:
            raise DuplicateMethodCall(method.__name__)
        self._called.add(method)
        method(self, *args, **kwargs)
        return self
    return inner


def test_wrapper(self: _Test, function: Callable[..., None]) -> Callable[[], None]:
    @wraps(function)
    def inner() -> None:

        errors: list[str] = []

        for _ in range(self._times):
            inputs = {
                k: v() if callable(v) else v
                for k, v
                in self._params.items()
            }

            try:
                function(**inputs)
            except TestFailed as e:
                header = "TEST FAILED:\n" if not errors else "  +\n"
                errors.append(
                    f"{header}  | {function.__name__}({inputs if inputs else ''}) should {self._desc or ''}:\n  | {e}"
                )

        if errors:
            raise TestFailed('\n'.join(errors))


    return inner


class Test:

    @staticmethod
    def given(**params) -> _Test:
        return _Test().given(**params)

    @staticmethod
    def should(description: str) -> _Test:
        return _Test().should(description)

    @staticmethod
    def repeat(times: int) -> _Test:
        return _Test().repeat(times)



class _Test:

    _desc: str | None
    _params: dict[str, Any]
    _called: set[Callable]
    _times: int

    def __init__(self) -> None:
        self._desc = None
        self._params = dict()
        self._called = set()
        self._times = 1

    @chain_wrapper
    def should(self, description: str) -> None:
        self._desc = description

    @chain_wrapper
    def given(self, **params: dict[str, Any]) -> None:
        self._params.update(params)

    @chain_wrapper
    def repeat(self, times: int) -> None:
        self._times = times

    def __call__(self, function: Callable[..., None]) -> Callable[[], None]:
        wrapped = test_wrapper(self, function)
        TAMARIN_TEST_LIST.append(wrapped)
        return wrapped

