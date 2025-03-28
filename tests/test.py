from random import randint

from Tamarin import Test, Assert
from _test_utils import main, kip_exec

single_digit = lambda: randint(0, 9)
integer = lambda: randint(0, 255)

@Test.should("be able to add 1 and 1")
def test_one_plus_one():
    Assert(kip_exec("1 + 1")).equals("2\n".encode())

@Test.should("perform single digit addition").given(a = single_digit, b = single_digit).repeat(10)
def test_digit_addition(a, b):
    Assert(kip_exec(f"{a} + {b}")).equals(f"{a + b}\n".encode())

@Test.should("perform addition on integers").given(a = integer, b = integer).repeat(10)
def test_integer_addition(a, b):
    Assert(kip_exec(f"{a} + {b}")).equals(f"{a + b}\n".encode())

if __name__ == "__main__":
    main()

