#!/usr/bin/python3
#
# Testy pro 1. IZP projekt [2022]
# Autor: - Ramsay#2303
# Inspirace https://github.com/JosefKuchar/izp-projekt-1/blob/main/test.py
# Priklady pouziti:
#     python3 ./test.py t9search
#     python3 ./test.py t9search --bonus 2


import argparse
import json
from signal import SIGSEGV
from subprocess import CompletedProcess, run, PIPE
from typing import Dict, List, Tuple

TEST_LOG_FILENAME = "log.json"

PASS = "\033[38;5;154m[OK]\033[0m"
FAIL = "\033[38;5;196m[FAIL]\033[0m"

BLUE = "\033[38;5;12m"
BOLD = "\033[1m"
END = "\033[0m"

NOT_FOUND_MESSAGE = "Not found\n"

BASE_INPUT = [
    ("Petr Dvorak", "603123456"),
    ("Jana Novotna", "777987654"),
    ("Bedrich Smetana ml.", "541141120"),
    ("xxx+a+xxxxx", "213344"),
    ("Karel Spacek", "+420213333333"),
    ("aekaeeabbaeaebaeab", "892")
]

TOO_LONG_INPUT_1 = [
    ("X" * 101, "1"),
]

TOO_LONG_INPUT_2 = [
    ("X", "1" * 101),
]

BLANK_INPUT_1 = [
    ("", "1"),
]

BLANK_INPUT_2 = [
    ("X", ""),
]

MAX_CONTACTS_INPUT_1 = [(f"aa{i}aa", "25235453") for i in range(42)]

MAX_CONTACTS_INPUT_2 = [(f"aa{i}aa", "25235453") for i in range(50)]

FIRST_BONUS_INPUT = [
    ("xAxxxxBC", "123044312"),
    ("xxxABDxx", "3023296827"),
    ("xxOxxKxxOKxxxxZ", "00114322"),
    ("xmxvxxkxmxjxmxxxvxxtxxxm", "90010008")
]

SECOND_BONUS_INPUT = [
    ("Roman Orsag", "432923843"),
]


class Tester:
    def __init__(self, program_name: str, first_bonus: bool = False) -> None:
        self.program_name = "./" + program_name
        self.test_count = 0
        self.pass_count = 0
        self.first_bonus = first_bonus
        self.logs: List[Dict] = []

    def test(
        self,
        test_name: str,
        args: List[str],
        input_: List[Tuple[str, str]],
        expected_contacts: List[int] = None,
        should_fail: bool = False,
        check_crash: bool = False,
    ):
        self.test_count += 1
        failed = False
        error_msg: str = ""

        str_input = self.create_input(input_)
        str_output = (
            self.create_output(input_, expected_contacts)
            if expected_contacts is not None
            else ""
        )

        p: CompletedProcess[str]

        try:
            p = run(
                [self.program_name] + args,
                stdout=PIPE,
                stderr=PIPE,
                input=str_input,
                encoding="ascii",
            )
        except UnicodeEncodeError as e:
            self.print_fail(test_name)
            print("Vystup obsahuje znaky ktere nepatri do ASCII (napr. diakritika)")
            print(e)
        except Exception as e:
            self.print_fail(test_name)
            print("Chyba pri volani programu")
            print(e)
            exit(1)

        if p.returncode != 0:
            if p.returncode == -SIGSEGV and check_crash:
                failed = True
                error_msg += f"Program neocekavane spadl s navratovym kodem {p.returncode}. Pravdepodobne sahas do pameti ktera neni tvoje\n"
            elif not should_fail and not check_crash:
                failed = True
                error_msg += f"Program vratil chybovy navratovy kod {p.returncode} prestoze nemel\n"

        else:
            if should_fail:
                failed = True
                error_msg += "Program byl uspesne ukoncen, i presto ze nemel byt\n"

        if (
            not self.assert_equal(str_output, p.stdout)
            and not should_fail
            and not check_crash
        ):
            failed = True
            error_msg += "Vystup programu se neshoduje s ocekavanym vystupem"

        if should_fail and len(p.stderr) == 0:
            failed = True
            error_msg += "Program nevratil chybovou hlasku na STDERR\n"

        if failed:
            self.print_fail(test_name)
            print(error_msg)
            print(f"{self.bold('Argumenty')}: {' '.join(args)}")
            print(f"{self.bold('Predpokladany vystup')}:")
            print(self.debug(str_output))
            print(f"{self.bold('STDOUT')}:")
            print(self.debug(p.stdout))
            print(f"{self.bold('STDERR')}:")
            print(self.debug(p.stderr))
        else:
            self.pass_count += 1
            self.print_pass(test_name)

        data = {
            "test_name": test_name,
            "status": "failed" if failed else "ok",
            "error_message": error_msg,
            "args": " ".join(args),
            "exptected_output": str_output,
            "stdout": p.stdout,
            "stderr": p.stderr,
        }

        self.logs.append(data)

    def print_stats(self) -> None:
        success_rate = self.pass_count / self.test_count * 100
        print(
            self.bold(
                f"Uspesnost: {success_rate:.2f} % [{self.pass_count} / {self.test_count}]"
            )
        )

    def print_fail(self, msg: str) -> None:
        print(FAIL, msg)

    def print_pass(self, msg: str) -> None:
        print(PASS, msg)

    def assert_equal(self, output: str, expected_output: str) -> bool:
        lines = {line.lower() for line in expected_output.rstrip().split("\n")}

        for line in output.rstrip().split("\n"):
            line = line.lower()

            if line not in lines:
                return False

        return True

    def create_input(self, input_: List[Tuple[str, str]]) -> str:
        return "".join([f"{name}\n{number}\n" for name, number in input_])

    def create_output(
        self, input_: List[Tuple[str, str]], exptected_contacts: List[int]
    ) -> str:
        out = "" if len(exptected_contacts) else NOT_FOUND_MESSAGE
        for i, (name, number) in enumerate(input_):
            if i + 1 in exptected_contacts:
                name_boundary = len(name) if len(name) <= 100 else 100
                number_boundary = len(number) if len(number) <= 100 else 100

                out += f"{name.lower()[:name_boundary]}, {number.lower()[:number_boundary]}\n"

        return out

    def debug(self, text: str) -> str:
        return f"{BLUE}{text}{END}"

    def bold(self, text: str) -> str:
        return f"{BOLD}{text}{END}"

    def save_logs(self) -> None:
        with open(TEST_LOG_FILENAME, "w", encoding="utf8") as f:
            json.dump(self.logs, f, indent=4)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Tester 1. IZP projektu [2022]")
    parser.add_argument(
        "prog", metavar="P", type=str, help="Jmeno programu (napriklad: t9search)"
    )
    parser.add_argument(
        "--save-logs",
        dest="save_logs",
        action="store_true",
        help="Zapne ukladani logu do souboru",
    )
    parser.add_argument(
        "-b",
        "--bonus",
        dest="bonus",
        type=int,
        help="Kontrola bonusoveho zadani. Moznosti jsou 1 nebo 2",
    )

    args = parser.parse_args()

    bonus_level = args.bonus or 0

    t = Tester(args.prog, bonus_level > 0)

    t.test("Test ze zadani #1", [], BASE_INPUT, [1, 2, 3, 4, 5, 6])
    t.test("Test ze zadani #2", ["12"], BASE_INPUT, [1, 3])
    t.test("Test ze zadani #3", ["686"], BASE_INPUT, [2])
    t.test("Test ze zadani #4", ["38"], BASE_INPUT, [1, 3])
    t.test("Test ze zadani #5", ["111"], BASE_INPUT, [])

    t.test("Test standardniho reseni #1", ["020"], BASE_INPUT, [4])
    t.test("Test standardniho reseni #2", ["0420"], BASE_INPUT, [5])
    t.test("Test standardniho reseni #3", ["232"], BASE_INPUT, [6])
    t.test("Test standardniho reseni #4", ["779"], BASE_INPUT, [2])

    t.test(
        "Test maximalniho poctu kontaktu #1",
        [],
        MAX_CONTACTS_INPUT_1,
        [i for i in range(1, 43)],
    )
    t.test(
        "Test maximalniho poctu kontaktu #2",
        [],
        MAX_CONTACTS_INPUT_2,
        check_crash=True,
    )

    t.test("Test na delku radku #1", [], TOO_LONG_INPUT_1, check_crash=True)
    t.test("Test na delku radku #2", [], TOO_LONG_INPUT_2, check_crash=True)

    # TODO
    # t.test("Test na prazdny radek #1", [], BLANK_INPUT_1, [])
    # t.test("Test na prazdny radek #2", [], BLANK_INPUT_2, [])

    t.test("Test argumentu #1", ["tf"], BASE_INPUT, should_fail=True)
    t.test("Test argumentu #2", ["t00f"], BASE_INPUT, should_fail=True)
    t.test("Test argumentu #3", ["1231", "ff", "pp"], BASE_INPUT, should_fail=True)

    if bonus_level >= 1:
        t.test("Test na prvni rozsireni #1", ["-s", "222"], FIRST_BONUS_INPUT, [1, 2])
        t.test("Test na prvni rozsireni #2", ["-s", "226"], FIRST_BONUS_INPUT, [2])
        t.test("Test na prvni rozsireni #3", ["-s", "223"], FIRST_BONUS_INPUT, [2])
        t.test("Test na prvni rozsireni #4", ["-s", "892"], FIRST_BONUS_INPUT, [])
        t.test("Test na prvni rozsireni #5", ["-s", "659"], FIRST_BONUS_INPUT, [3, 4])
        t.test("Test na prvni rozsireni #6", ["-s", "688"], FIRST_BONUS_INPUT, [4])
        t.test("Test na prvni rozsireni #7", ["-s", "981"], FIRST_BONUS_INPUT, [])

        t.test(
            "Test parametru -s #1",
            ["892", "-s"],
            FIRST_BONUS_INPUT,
            should_fail=True,
        )
§
    if bonus_level == 2:
        t.test("Test na druhe rozsireni #1", ["62", "-l", "1"], SECOND_BONUS_INPUT, [1])
        t.test(
            "Test na druhe rozsireni #2", ["602", "-l", "1"], SECOND_BONUS_INPUT, [1]
        )
        t.test(
            "Test na druhe rozsireni #3", ["6620", "-l", "1"], SECOND_BONUS_INPUT, [1]
        )
        t.test(
            "Test na druhe rozsireni #4", ["6020", "-l", "1"], SECOND_BONUS_INPUT, []
        )
        t.test(
            "Test na druhe rozsireni #5", ["6020", "-l", "2"], SECOND_BONUS_INPUT, [1]
        )
        t.test(
            "Test na druhe rozsireni #6", ["626", "-l", "1"], SECOND_BONUS_INPUT, [1]
        )
        t.test(
            "Test na druhe rozsireni #7", ["626", "-l", "1"], SECOND_BONUS_INPUT, [1]
        )
        t.test(
            "Test na druhe rozsireni #8", ["662", "-l", "0"], SECOND_BONUS_INPUT, [1]
        )
        t.test("Test na druhe rozsireni #9", ["660", "-l", "0"], SECOND_BONUS_INPUT, [])

        t.test(
            "Test parametru -l #1",
            ["-l", "tf"],
            SECOND_BONUS_INPUT,
            should_fail=True,
        )
        t.test(
            "Test parametru -l #2",
            ["-l", "t00f"],
            SECOND_BONUS_INPUT,
            should_fail=True,
        )
        t.test("Test parametru -l #3", ["-l"], SECOND_BONUS_INPUT, should_fail=True)
        t.test("Test parametru -l #4", ["-l", "0", "662"], SECOND_BONUS_INPUT, [1])

    if args.save_logs:
        t.save_logs()

    t.print_stats()
