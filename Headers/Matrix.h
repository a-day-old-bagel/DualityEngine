/*******************************************************************************
 * File:   Matrix.h
 * Author: Galen Cochrane *
 * Created on July 9, 2015, 12:32 AM
 * 
 * 
 ******************************************************************************/

#ifndef DUA_MATRIX_H
#define	DUA_MATRIX_H

namespace DualityEngine{

    template<int _rows, int _cols, class _T>
    class Matrix{
    private:
        _T value[_rows][_cols];
    public:
        Matrix();
        Matrix(_T scalar);
        Matrix(std::initializer_list<_T> list);
//        template<typename ...ARGT>
//        Matrix(ARGT&&...args);
        ~Matrix();
        
//        template<int rhs_rows, int rhs_cols, class rhs_T>
//        void operator = (Matrix<rhs_rows, rhs_cols, rhs_T> rhs);
        
        _T& operator()(int row, int col);
        
        template<int rhs_rows, int rhs_cols, class rhs_T>
        Matrix<_rows, rhs_cols, _T> operator * (Matrix<rhs_rows, rhs_cols, rhs_T> rhs);
        
        Matrix<_rows, _cols, _T> operator * (int scalar);
        Matrix<_rows, _cols, _T> operator * (float scalar);
        Matrix<_rows, _cols, _T> operator * (double scalar);
    };
    
    
    typedef Matrix< 4, 4, float>    Mat4x4f;
    typedef Matrix< 4, 4, double>   Mat4x4d;
    typedef Matrix< 3, 3, float>    Mat3x3f;
    typedef Matrix< 3, 3, double>   Mat3x3d;
    
    typedef Matrix< 4, 1, float>    Vec4f;
    typedef Matrix< 4, 1, double>   Vec4d;
    typedef Matrix< 3, 1, float>    Vec3f;
    typedef Matrix< 3, 1, double>   Vec3d;

}

#endif	/* DUA_MATRIX_H */

