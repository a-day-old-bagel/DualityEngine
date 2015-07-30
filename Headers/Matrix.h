/*******************************************************************************
 * File:   Matrix.h
 * Author: Galen Cochrane *
 * Created on July 9, 2015, 12:32 AM
 * 
 * 
 ******************************************************************************/

#ifndef DUA_MATRIX_H
#define	DUA_MATRIX_H

#include <initializer_list>
#include <cmath>
#include <cassert>
#include <algorithm>

namespace DualityEngine{

                                                                /*MATRIX DECLARATIONS*/
    template<int _rows, int _cols, class _T>
    class Matrix{
    private:
        _T value[_rows][_cols];
    public:
        Matrix();
        Matrix(_T scalar);
//        Matrix(std::initializer_list<_T> list);
        Matrix(std::initializer_list<std::initializer_list<_T>> list);
//        template<typename ...ARGT>
//        Matrix(ARGT&&...args);
        ~Matrix();
        
//        template<int rhs_rows, int rhs_cols, class rhs_T>
//        void operator = (Matrix<rhs_rows, rhs_cols, rhs_T> rhs);
        
        _T& operator()(const int row, const int col);
        
        template<int rhs_rows, int rhs_cols, class rhs_T>
        Matrix<_rows, rhs_cols, _T> operator * (Matrix<rhs_rows, rhs_cols, rhs_T> rhs);
        
//        template<int rhs_rows, int rhs_cols, class rhs_T>
//        void operator *= (Matrix<rhs_rows, rhs_cols, rhs_T> rhs);
        
        Matrix<_rows, _cols, _T> operator * (int scalar);
        Matrix<_rows, _cols, _T> operator * (float scalar);
        Matrix<_rows, _cols, _T> operator * (double scalar);
        void operator *= (int scalar);
        void operator *= (float scalar);
        void operator *= (double scalar);
        
        Matrix<_rows, _cols, _T> returnMultByThis (double scalar);
        void doMultByThis(double scalar);
        
        #ifdef DUA_DEBUG_MATRIX_PRINT
        void print(){
            std::string printString = "";
            for (int i = 0; i < _rows; ++i){
                for (int j = 0; j < _cols; ++j){
                    printString += std::to_string(value[i][j]) + " ";
                }
                printString += '\n';
            }
            printString += '\n';
            std::cout << printString;
        }
        #endif
    };
    
    
    
    
    
                                                                /*MATRIX TYPEDEFS*/
    typedef Matrix< 4, 4, float>    Mat4x4f;
    typedef Matrix< 4, 4, double>   Mat4x4d;
    typedef Matrix< 3, 3, float>    Mat3x3f;
    typedef Matrix< 3, 3, double>   Mat3x3d;
    
    typedef Matrix< 4, 1, float>    Vec4f;
    typedef Matrix< 4, 1, double>   Vec4d;
    typedef Matrix< 3, 1, float>    Vec3f;
    typedef Matrix< 3, 1, double>   Vec3d;
    
    
    
    
    
                                                                /*MATRIX DEFINITIONS*/
    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T>::Matrix() : Matrix(0) {

    }

    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T>::Matrix(_T scalar) {
        for (int i = 0; i < _rows; ++i) {
            for (int j = 0; j < _cols; ++j) {
                value[i][j] = scalar;
            }
        }
    }
    
//    template<int _rows, int _cols, class _T>
//    Matrix<_rows, _cols, _T>::Matrix(std::initializer_list<_T> list) {
//        _T* iterator = list.begin();
//        for (int i = 0; i < _rows; ++i){
//            for (int j = 0; j < _cols; ++j){
//                value[i][j] = *(iterator++);
//            }
//        }
//    }
    
    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T>::Matrix(std::initializer_list<std::initializer_list<_T>> list){
        for (int i = 0; i < _rows; ++i){
            std::copy((list.begin() + i)->begin(), (list.begin() + i)->end(), value[i]);
        }
    }

    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T>::~Matrix() {

    }

    //template<int _rows, int _cols, class _T>
    //template<int rhs_rows, int rhs_cols, class rhs_T>
    //void Matrix<_rows, _cols, _T>::operator =(Matrix<rhs_rows,rhs_cols,rhs_T> rhs){
    //    
    //}

    template<int _rows, int _cols, class _T>
    template<int rhs_rows, int rhs_cols, class rhs_T>
    Matrix<_rows, rhs_cols, _T> Matrix<_rows, _cols, _T>::operator*(Matrix<rhs_rows, rhs_cols, rhs_T> rhs) {
        // ASSERT HERE
        Matrix<_rows, rhs_cols, _T> result;
        for (int i = 0; i < _rows; ++i) {
            for (int j = 0; j < rhs_cols; ++j) {
                for (int k = 0; k < _cols; ++k) {
                    result(i,j) += value[i][k] * rhs(k,j);
                }
            }
        }
        return result;
    }

    template<int _rows, int _cols, class _T>
    _T& Matrix<_rows, _cols, _T>::operator()(const int row, const int col) {
        return value[row][col];
    }

    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T> Matrix<_rows, _cols, _T>::operator*(int scalar) {
        return returnMultByThis((double)scalar);
    }

    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T> Matrix<_rows, _cols, _T>::operator*(float scalar) {
        return returnMultByThis((double)scalar);
    }

    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T> Matrix<_rows, _cols, _T>::operator*(double scalar) {
        return returnMultByThis(scalar);
    }
    
    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T> Matrix<_rows, _cols, _T>::returnMultByThis(double scalar){
        Matrix<_rows, _cols, _T> result;
        for (int i = 0; i < _rows; ++i) {
            for (int j = 0; j < _cols; ++j) {
                result.value[i][j] = this->value[i][j] * scalar;
            }
        }
        return result;
    }
    
    
    
    
    template<int _rows, int _cols, class _T>
    void Matrix<_rows, _cols, _T>::operator*= (int scalar) {
        return doMultByThis((double)scalar);
    }

    template<int _rows, int _cols, class _T>
    void Matrix<_rows, _cols, _T>::operator*= (float scalar) {
        return doMultByThis((double)scalar);
    }

    template<int _rows, int _cols, class _T>
    void Matrix<_rows, _cols, _T>::operator*= (double scalar) {
        return doMultByThis(scalar);
    }
    

    template<int _rows, int _cols, class _T>
    void Matrix<_rows, _cols, _T>::doMultByThis(double scalar){
        for (int i = 0; i < _rows; ++i) {
            for (int j = 0; j < _cols; ++j) {
                value[i][j] *= scalar;
            }
        }
    }
    
}

#endif	/* DUA_MATRIX_H */

