#include "../Headers/Matrix.h"

using namespace DualityEngine;

template<int _rows, int _cols, class _T>
Matrix<_rows, _cols, _T>::Matrix(){
    *this = Matrix(0);
}

template<int _rows, int _cols, class _T>
Matrix<_rows, _cols, _T>::Matrix(_T scalar){
    for (int i = 0; i < _rows; ++i){
        for (int j = 0; j < _cols; ++j){
            value[i][j] = scalar;
        }
    }
}

template<int _rows, int _cols, class _T>
Matrix<_rows, _cols, _T>::~Matrix(){
    
}

//template<int _rows, int _cols, class _T>
//template<int rhs_rows, int rhs_cols, class rhs_T>
//void Matrix<_rows, _cols, _T>::operator =(Matrix<rhs_rows,rhs_cols,rhs_T> rhs){
//    
//}

template<int _rows, int _cols, class _T>
template<int rhs_rows, int rhs_cols, class rhs_T>
Matrix<_rows, rhs_cols, _T> Matrix<_rows, _cols, _T>::operator *(Matrix<rhs_rows,rhs_cols,rhs_T> rhs){
    Matrix<_rows, rhs_cols, _T> result(0);
    for (int i = 0; i < _rows; ++i){
        for (int j = 0; j < rhs_cols; ++j){
            for (int k = 0; k < _cols; ++k){
                result.value[i][j] += this->value[i][k] * rhs.value[k][j];
            }
        }
    }
}

template<int _rows, int _cols, class _T>
_T& Matrix<_rows, _cols, _T>::operator ()(int row, int col){
    return value[row][col];
}

template<int _rows, int _cols, class _T>
Matrix<_rows, _cols, _T> Matrix<_rows, _cols, _T>::operator *(int scalar){
    return this * ((double)scalar);
}

template<int _rows, int _cols, class _T>
Matrix<_rows, _cols, _T> Matrix<_rows, _cols, _T>::operator *(float scalar){
    return this * ((double)scalar);
}

template<int _rows, int _cols, class _T>
Matrix<_rows, _cols, _T> Matrix<_rows, _cols, _T>::operator *(double scalar){
    for (int i = 0; i < _rows; ++i){
        for (int j = 0; j < _cols; ++j){
            value[i][j] *= scalar;
        }
    }
}