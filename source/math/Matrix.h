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
#include <iostream>
#include "settings.h"

namespace DualityEngine{
                                                                /*MATRIX DECLARATIONS*/
    template<int _rows, int _cols, class _T>
    class Matrix{
    private:
        _T value[_rows][_cols];
    public:
        Matrix();
        Matrix(_T scalar);
        Matrix(std::initializer_list<std::initializer_list<_T>> list);
//        template<typename ...ARGT>
//        Matrix(ARGT&&...args);
        ~Matrix();
        
        _T& operator()(const int row, const int col);
        
        template<int rhs_rows, int rhs_cols, class rhs_T>
        Matrix<_rows, rhs_cols, _T> operator * (Matrix<rhs_rows, rhs_cols, rhs_T> rhs);
        
        template<int rhs_rows, int rhs_cols, class rhs_T>
        void leftMultiply(Matrix<rhs_rows, rhs_cols, rhs_T> rhs);
        
        Matrix<_rows, _cols, _T> operator * (int scalar);
        Matrix<_rows, _cols, _T> operator * (float scalar);
        Matrix<_rows, _cols, _T> operator * (double scalar);
        void operator *= (int scalar);
        void operator *= (float scalar);
        void operator *= (double scalar);
        
        Matrix<_rows, _cols, _T> returnMultByThis (DUA_matrixVal scalar);
        void doMultByThis(DUA_matrixVal scalar);
        
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

    typedef Matrix< 4, 4, DUA_matrixVal>   Mat4x4;
    typedef Matrix< 3, 3, DUA_matrixVal>   Mat3x3;    
    typedef Matrix< 4, 1, DUA_matrixVal>   Vec4;
    typedef Matrix< 3, 1, DUA_matrixVal>   Vec3;
    
    
    
    
    
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
    
    
    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T>::Matrix(std::initializer_list<std::initializer_list<_T>> list){
        for (int i = 0; i < _rows; ++i){
            std::copy((list.begin() + i)->begin(), (list.begin() + i)->end(), value[i]);
        }
    }

    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T>::~Matrix() {

    }

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
    template<int rhs_rows, int rhs_cols, class rhs_T>
    void Matrix<_rows, _cols, _T>::leftMultiply(Matrix<rhs_rows, rhs_cols, rhs_T> rhs) {
        *this = rhs * *this;
    }

    template<int _rows, int _cols, class _T>
    _T& Matrix<_rows, _cols, _T>::operator()(const int row, const int col) {
        return value[row][col];
    }

    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T> Matrix<_rows, _cols, _T>::operator*(int scalar) {
        return returnMultByThis((DUA_matrixVal)scalar);
    }

    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T> Matrix<_rows, _cols, _T>::operator*(float scalar) {
        return returnMultByThis((DUA_matrixVal)scalar);
    }

    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T> Matrix<_rows, _cols, _T>::operator*(double scalar) {
        return returnMultByThis((DUA_matrixVal)scalar);
    }
    
    template<int _rows, int _cols, class _T>
    Matrix<_rows, _cols, _T> Matrix<_rows, _cols, _T>::returnMultByThis(DUA_matrixVal scalar){
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
        return doMultByThis((DUA_matrixVal)scalar);
    }

    template<int _rows, int _cols, class _T>
    void Matrix<_rows, _cols, _T>::operator*= (float scalar) {
        return doMultByThis((DUA_matrixVal)scalar);
    }

    template<int _rows, int _cols, class _T>
    void Matrix<_rows, _cols, _T>::operator*= (double scalar) {
        return doMultByThis((DUA_matrixVal)scalar);
    }
    

    template<int _rows, int _cols, class _T>
    void Matrix<_rows, _cols, _T>::doMultByThis(DUA_matrixVal scalar){
        for (int i = 0; i < _rows; ++i) {
            for (int j = 0; j < _cols; ++j) {
                value[i][j] *= scalar;
            }
        }
    }
    
}

#endif	/* DUA_MATRIX_H */

