// Talles Siqueira Ceolin
// Programação Paralela - T3

#include <algorithm>
#include <iostream>
#include <string>
#include <omp.h>

class SharedArray {
   private:
        char* array;
        int index;
        int size;
        bool usemutex;
        
        void spendSomeTime() {
            for (int i = 0; i < 10000; i++) {
                for (int j = 0; j < 100; j++) {
                    // These loops shouldn't be removed by the compiler
                }
            }
        }
   public:
        SharedArray(int n, bool use) : size(n), index(0), usemutex(use) {
            array = new char[size];
            std::fill(array, array+size, '-');
        }
        ~SharedArray() {
            delete[] array;
        }
        void addChar(char c) {
            if (usemutex){
                #pragma omp ordered
                {
                    array[index] = c;
                    spendSomeTime();
                    index++;
                }
            }else{
                array[index] = c;
                spendSomeTime();
                index++;
            }
        }
        int countOccurrences(char c) {
            return std::count(array, array+size, c);
        }
        std::string toString() {
            return std::string(array, size);
        } 
};


class ArrayFiller {
    private:
        static const int nThreads = 3;
        static const int nTimes = 20;
        SharedArray* array;

        void run(char c) {
            for (int i = 0; i < nTimes; i++) {
                array->addChar(c);
            }
        }
    public:
        ArrayFiller(bool usemutex, omp_sched_t schedule, int chunks) {
            array = new SharedArray(nThreads * nTimes, usemutex);
            omp_set_schedule(schedule, chunks);
        }
        
        ArrayFiller(bool usemutex) { // construtor para o runtime
            array = new SharedArray(nThreads * nTimes, usemutex);
        }

        void fillArrayConcurrently() {

            omp_set_num_threads(nThreads);

            #pragma omp parallel for schedule(runtime) ordered
            for (int i = 0; i < nThreads * nTimes; ++i){
                array->addChar('A' + omp_get_thread_num());
            }

        }

        void printStats() {
            std::cout << "\033[1;32m" << array->toString() << " \033[1;37m";
            for (int i = 0; i < nThreads; ++i)
                std::cout << (char) ('A' + i) << "=" << array->countOccurrences('A'+i) << " ";
            std::cout << std::endl;
        }

        ~ArrayFiller() {
            delete array;
        }
};


int main() {
    // runtime com mutex
    std::cout << "\033[1;36mruntime \033[1;33mchunkless   \033[1;31mMUTEX ";
    ArrayFiller AF_runTimeM(true);
    AF_runTimeM.fillArrayConcurrently();
    AF_runTimeM.printStats();

    // runtime sem mutex
    std::cout << "\033[1;36mruntime \033[1;33mchunkless \033[1;31mnoMUTEX ";
    ArrayFiller AF_runTime(false);
    AF_runTime.fillArrayConcurrently();
    AF_runTime.printStats();

    // static sem chunk com mutex
    std::cout << "\033[1;36mstatic  \033[1;33mchunkless   \033[1;31mMUTEX ";
    ArrayFiller AF_staticChunklessM(true, omp_sched_static, 0);
    AF_staticChunklessM.fillArrayConcurrently();
    AF_staticChunklessM.printStats();

    // static com chunk com mutex
    std::cout << "\033[1;36mstatic  \033[1;33mchunk = 4   \033[1;31mMUTEX ";
    ArrayFiller AF_staticChunkM(true, omp_sched_static, 4);
    AF_staticChunkM.fillArrayConcurrently();
    AF_staticChunkM.printStats();

    // static sem chunk sem mutex
    std::cout << "\033[1;36mstatic  \033[1;33mchunkless \033[1;31mnoMUTEX ";
    ArrayFiller AF_staticChunkless(false, omp_sched_static, 0);
    AF_staticChunkless.fillArrayConcurrently();
    AF_staticChunkless.printStats();

    // static com chunk sem mutes
    std::cout << "\033[1;36mstatic  \033[1;33mchunk = 4 \033[1;31mnoMUTEX ";
    ArrayFiller AF_staticChunk(false, omp_sched_static, 4);
    AF_staticChunk.fillArrayConcurrently();
    AF_staticChunk.printStats();

    // dynamic sem chunk com mutex
    std::cout << "\033[1;36mdynamic \033[1;33mchunkless   \033[1;31mMUTEX ";
    ArrayFiller AF_dynamicChunklessM(true, omp_sched_dynamic, 0);
    AF_dynamicChunklessM.fillArrayConcurrently();
    AF_dynamicChunklessM.printStats();

    // dynamic com chunk com mutex
    std::cout << "\033[1;36mdynamic \033[1;33mchunk = 4   \033[1;31mMUTEX ";
    ArrayFiller AF_dynamicChunkM(true, omp_sched_dynamic, 4);
    AF_dynamicChunkM.fillArrayConcurrently();
    AF_dynamicChunkM.printStats();

    // dynamic sem chunk sem mutex
    std::cout << "\033[1;36mdynamic \033[1;33mchunkless \033[1;31mnoMUTEX ";
    ArrayFiller AF_dynamicChunkless(false, omp_sched_dynamic, 0);
    AF_dynamicChunkless.fillArrayConcurrently();
    AF_dynamicChunkless.printStats();

    // dynamic com chunk sem mutex
    std::cout << "\033[1;36mdynamic \033[1;33mchunk = 4 \033[1;31mnoMUTEX ";
    ArrayFiller AF_dynamicChunk(false, omp_sched_dynamic, 4);
    AF_dynamicChunk.fillArrayConcurrently();
    AF_dynamicChunk.printStats();

    // guided sem chunk com mutex
    std::cout << "\033[1;36mguided  \033[1;33mchunkless   \033[1;31mMUTEX ";
    ArrayFiller AF_guidedChunklessM(true, omp_sched_guided, 0);
    AF_guidedChunklessM.fillArrayConcurrently();
    AF_guidedChunklessM.printStats();

    // guided com chunk com mutex
    std::cout << "\033[1;36mguided  \033[1;33mchunk = 4   \033[1;31mMUTEX ";
    ArrayFiller AF_guidedChunkM(true, omp_sched_guided, 4);
    AF_guidedChunkM.fillArrayConcurrently();
    AF_guidedChunkM.printStats();
    
    // guided sem chunk sem mutex
    std::cout << "\033[1;36mguided  \033[1;33mchunkless \033[1;31mnoMUTEX ";
    ArrayFiller AF_guidedChunkless(false, omp_sched_guided, 0);
    AF_guidedChunkless.fillArrayConcurrently();
    AF_guidedChunkless.printStats();

    // guided com chunk sem mutex
    std::cout << "\033[1;36mguided  \033[1;33mchunk = 4 \033[1;31mnoMUTEX ";
    ArrayFiller AF_guidedChunk(false, omp_sched_guided, 4);
    AF_guidedChunk.fillArrayConcurrently();
    AF_guidedChunk.printStats();

    // auto com mutex
    std::cout << "\033[1;36mauto    \033[1;33mchunkless   \033[1;31mMUTEX ";
    ArrayFiller AF_autoChunklessM(true, omp_sched_auto, 0);
    AF_autoChunklessM.fillArrayConcurrently();
    AF_autoChunklessM.printStats();   

    // auto sem mutex
    std::cout << "\033[1;36mauto    \033[1;33mchunkless \033[1;31mnoMUTEX ";
    ArrayFiller AF_autoChunkless(false, omp_sched_auto, 0);
    AF_autoChunkless.fillArrayConcurrently();
    AF_autoChunkless.printStats();   
}
