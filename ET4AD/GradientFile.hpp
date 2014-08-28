/* 
 * File:   GradientFile.hpp
 * Author: matthewsupernaw
 *
 * Created on August 26, 2014, 4:18 PM
 */

#ifndef GRADIENTFILE_HPP
#define	GRADIENTFILE_HPP

#include <fstream>

namespace et4ad {

    using std::strerror;
    using std::cerr;

    template<class T, int group = 0, size_t MAX_GRADIENT_SIZE = 1000 >
    class GradientFile {
        template<class REAL_T, int g, size_t MGS> friend GradientFile<REAL_T, g, MGS> * GradientFileInstance();
        uint32_t id;
        mutable uint32_t d_index;

        T zero;

        T zeros[MAX_GRADIENT_SIZE];

        mutable std::fstream data;
        static GradientFile<T, group, MAX_GRADIENT_SIZE> *singleton;
        T getter;

        GradientFile() : id(0), d_index(0), zero(0) {
            getter = 0;
            memset(zeros, 0, sizeof (zeros));
            std::stringstream ss;
            ss << "gradient_" << sizeof (T)*8 << "bit_group" << group << "_ind" << MAX_GRADIENT_SIZE << ".bin";
            data.open(ss.str().c_str(), std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

            if (!data.is_open()) {
                std::cout << "error creating gradient disk storage file named " << ss.str() << "!";
                exit(0);
            }
        }





    public:

        inline uint32_t Next() {
            d_index = id;
            uint32_t ind;
            ind = sizeof (T)*(d_index * MAX_GRADIENT_SIZE);
            data.seekp(ind);
            data.write(reinterpret_cast<char *> (&zeros), sizeof (T) * MAX_GRADIENT_SIZE);
            id++;

            return d_index;
        }

        inline const T& Get(const uint32_t& id, const uint32_t& index) {

            data.seekg(sizeof (T)*(id * MAX_GRADIENT_SIZE + index));
            data.read(reinterpret_cast<char *> (&getter), sizeof (T));
            return getter;

        }

        inline const T& Get(const uint32_t& id, const uint32_t& index) const {
            data.seekg(sizeof (T)*(id * MAX_GRADIENT_SIZE + index));
            data.read(reinterpret_cast<char *> (&getter), sizeof (T));

            return getter;

        }

        inline void Set(const uint32_t& id, const uint32_t& index, T& val) {
            data.seekp(sizeof (T)*(id * MAX_GRADIENT_SIZE + index));
            data.write(reinterpret_cast<char *> (&val), sizeof (T));
        }

        inline void Set(const uint32_t& id, const uint32_t& index, T& val) const {
            data.seekp(sizeof (T)*(id * MAX_GRADIENT_SIZE + index));
            data.write(reinterpret_cast<char *> (&val), sizeof (T));
        }
        
        void Reset(const uint32_t& id){
            uint32_t ind = sizeof (T)*(id * MAX_GRADIENT_SIZE);
            data.seekp(ind);
            data.write(reinterpret_cast<char *> (&zeros), sizeof (T) * MAX_GRADIENT_SIZE);
        }



    };

    template<class T, int group, size_t MAX_GRADIENT_SIZE>
    GradientFile<T, group, MAX_GRADIENT_SIZE>* GradientFile<T, group, MAX_GRADIENT_SIZE>::singleton = NULL;

    template<class REAL_T, int g, size_t MAX_GRADIENT_SIZE>
    static GradientFile<REAL_T, g, MAX_GRADIENT_SIZE> * GradientFileInstance() {


        if (GradientFile<REAL_T, g, MAX_GRADIENT_SIZE>::singleton == NULL) {
            GradientFile<REAL_T, g, MAX_GRADIENT_SIZE>::singleton = new GradientFile<REAL_T, g, MAX_GRADIENT_SIZE > ();
        }

        return GradientFile<REAL_T, g, MAX_GRADIENT_SIZE>::singleton;

    }



}


#endif	/* GRADIENTFILE_HPP */

