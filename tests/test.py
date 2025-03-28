from Tamarin import Test, Assert, run_tests
from test_utils import compile, CompileError


if __name__ == "__main__":
    try:
        compile()
        run_tests()
    except CompileError as e:
        print(e)

