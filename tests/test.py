from random import randint

from Tamarin import Test, Assert
from _test_utils import main, kip_exec

single_digit = lambda: randint(0, 9)
integer = lambda: randint(0, 255)
big_int = lambda: randint(125, 255)
small_int = lambda: randint(0, 123)

@Test.should("be able to add 1 and 1")
def test_one_plus_one():
    Assert(kip_exec("1 + 1")).equals("2\n".encode())

@Test.should("perform single digit addition").given(a = single_digit, b = single_digit).repeat(10)
def test_digit_addition(a, b):
    Assert(kip_exec(f"{a} + {b}")).equals(f"{a + b}\n".encode())

@Test.should("perform addition on integers").given(a = integer, b = integer).repeat(10)
def test_integer_addition(a, b):
    Assert(kip_exec(f"{a} + {b}")).equals(f"{a + b}\n".encode())

@Test.should("perform subtraction on integers").given(a = big_int, b = small_int).repeat(10)
def test_integer_subtraction(a, b):
    Assert(kip_exec(f"{a} - {b}")).equals(f"{a - b}\n".encode())

@Test.should("perform multiplicaiton on integers").given(a = small_int, b = small_int).repeat(10)
def test_integer_multiplicaiton(a, b):
    Assert(kip_exec(f"{a} * {b}")).equals(f"{a * b}\n".encode())

@Test.should("fail on ill formed binary expressions")
def test_bad_expr():
    Assert(
        kip_exec(f"1 + nope", stderr=True)
    ).equals(
        b'Parsing Failed:\n  | 1 + nope\n  | ~~~~^ Expected an Integer.\n'
    )

@Test.should("correctly compute simple compound expressions").given(a = small_int, b = small_int, c = small_int).repeat(10)
def test_simple_compound_expressions(a, b, c):
    Assert(kip_exec(f"{a} * {b} - {c}")).equals(f"{a * b - c}\n".encode())

@Test.should("correctly compute non-trivial compound expressions").given(a = small_int, b = small_int, c = small_int).repeat(10)
def test_harder_compound_expressions(a, b, c):
    Assert(kip_exec(f"{a} - {b} * {c}")).equals(f"{a - b * c}\n".encode())


if __name__ == "__main__":
    main()

