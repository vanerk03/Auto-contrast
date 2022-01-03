import os


class Test:
    def __init__(self, threads: int, input_filename: str, output_filename: str, coef: float):
        self.__threads = threads
        self.__input_filename = input_filename
        self.__output_filename = output_filename
        self.__coef = coef

    def run(self):
        os.system(
            f".\hw5 {self.__threads} {self.__input_filename} {self.__output_filename} {self.__coef}")


class Tester:
    def __init__(self, tests: list[Test]) -> None:
        self.__tests = tests

    def run_many(self):
        # compile()
        for test in self.__tests:
            test.run()


def compile(omp=True):
    if omp:
        os.system(f"g++ -fopenmp hw5.cpp -o hw5 -O3")
    else:
        os.system(f"g++ hw5.cpp -o hw5 -O3")


# file = open("test.txt", "w")
# file.write("")
# file.close()

compile(omp=True)
test1 = Test(1, "tests//test100mb.ppm", "output.ppm", 0.1)

for i in range(20):
    test1.run()
