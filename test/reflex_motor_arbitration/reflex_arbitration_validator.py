# Copyright (c) 2026 David Such
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

from itertools import product

def logic(AVOID_L, AVOID_R, APP_L, APP_R):
    # Derived signals
    AVOID_ANY = AVOID_L or AVOID_R
    APP_ANY   = APP_L or APP_R

    # Avoid direction terms
    DIR_L_avoid = AVOID_R and not AVOID_L
    DIR_R_avoid = AVOID_L and not AVOID_R

    # Approach direction terms
    DIR_L_app = APP_R
    DIR_R_app = APP_L

    # Final direction equations
    DIR_L = DIR_L_avoid or (not AVOID_ANY and APP_ANY and DIR_L_app)
    DIR_R = DIR_R_avoid or (not AVOID_ANY and APP_ANY and DIR_R_app)

    # Enable logic (simplified: enabled if any avoid or approach)
    EN_L = AVOID_ANY or APP_ANY
    EN_R = AVOID_ANY or APP_ANY

    return EN_L, DIR_L, EN_R, DIR_R


def expected(AVOID_L, AVOID_R, APP_L, APP_R):
    # Expected behavior based on design intent

    if AVOID_L or AVOID_R:
        # Avoid takes priority
        if AVOID_L and AVOID_R:
            return True, 0, True, 0      # back away
        elif AVOID_L:
            return True, 0, True, 1      # turn right
        else:
            return True, 1, True, 0      # turn left

    if APP_L or APP_R:
        # Approach behavior
        if APP_L and APP_R:
            return True, 1, True, 1      # forward
        elif APP_L:
            return True, 0, True, 1      # turn left
        else:
            return True, 1, True, 0      # turn right

    return False, 0, False, 0            # no motion


def test():
    print(f"{'AV_L':>4} {'AV_R':>4} {'AP_L':>4} {'AP_R':>4} | "
          f"{'EN_L':>4} {'DIR_L':>5} {'EN_R':>4} {'DIR_R':>5} | "
          f"{'Expected':>20} | Result")
    print("-" * 90)

    for AVOID_L, AVOID_R, APP_L, APP_R in product([0,1], repeat=4):
        actual = logic(AVOID_L, AVOID_R, APP_L, APP_R)
        exp    = expected(AVOID_L, AVOID_R, APP_L, APP_R)

        status = "PASS" if actual == exp else "FAIL"

        print(f"{AVOID_L:>4} {AVOID_R:>4} {APP_L:>4} {APP_R:>4} | "
              f"{actual[0]:>4} {actual[1]:>5} {actual[2]:>4} {actual[3]:>5} | "
              f"{str(exp):>20} | {status}")


if __name__ == "__main__":
    test()