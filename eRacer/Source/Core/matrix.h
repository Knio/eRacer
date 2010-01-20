#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include "point.h"

template <typename T=float>
struct MatrixType
{
    #pragma warning(disable : 4201) // anonymous union/struct
    union 
    {
        T E[16];
        T A[4][4]; 
        struct 
        {
            T   m11, m21, m31, m41,
                m12, m22, m32, m42,
                m13, m23, m33, m43, 
                m14, m24, m34, m44;
        };
    }; 
    
    MatrixType()
    {
        for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)            
            A[i][j] = i==j;
    }

    MatrixType(
        T a11, T a12, T a13, T a14,
        T a21, T a22, T a23, T a24,              
        T a31, T a32, T a33, T a34,
        T a41, T a42, T a43, T a44)
    {
        m11 = a11; m12 = a12; m13 = a13; m14 = a14;
        m21 = a21; m22 = a22; m23 = a23; m24 = a24;
        m31 = a31; m32 = a32; m33 = a33; m34 = a34;
        m41 = a41; m42 = a42; m43 = a43; m44 = a44;
    }



    template <typename U>
    MatrixType(const U* B)
    {
        for (int i=0;i<16;i++) 
            E[i] = static_cast<T>(B[i]);
    }
    
    template <typename U>
    MatrixType(const MatrixType<U>& B)
    {
        for (int i=0;i<16;i++) 
            E[i] = static_cast<T>(B.E[i]);
    }

	MatrixType& operator=(const MatrixType &B)
	{
		for (int i=0;i<16;i++) 
            E[i] = B.E[i];
        return *this;
	}
	
    const T* operator[](int i) const { return  A[i]; }
          T* operator[](int i)       { return  A[i]; }

    operator const T*() const { return E; }
    operator       T*()       { return E; }
       
    MatrixType operator*(const MatrixType &B) const
    {
        MatrixType R;
        for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            R[i][j] = A[i][0] * B[0][j] +
                      A[i][1] * B[1][j] +                      
                      A[i][2] * B[2][j] +                      
                      A[i][3] * B[3][j];
        return R;
    }

    MatrixType& operator*=(const MatrixType &B)
    {
        MatrixType R;
        for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            R[i][j] = A[i][0] * B[0][j] +
                      A[i][1] * B[1][j] +                      
                      A[i][2] * B[2][j] +                      
                      A[i][3] * B[3][j];
        *this = R;
        return *this;
    }

    template <typename U>
    PointType<U> operator*(const PointType<U> &B) const
    {
        PointType<U> R;
        for (int i=0;i<4;i++)
            R[i] = A[i][0] * B[0] +
                   A[i][1] * B[1] +                      
                   A[i][2] * B[2] +                      
                   A[i][3] * B[3];
        return R;
    }
    
    MatrixType transpose() const
    {
        MatrixType B;
        for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            B[j][i] = A[i][j];
        return B;
    }


    MatrixType inverse() const
    { // from http://webcvs.freedesktop.org/mesa/Mesa/src/glu/sgi/libutil/project.c?revision=1.6&view=markup
        int i, j, k;
        float t;
        
        MatrixType I;
        MatrixType A = *this;
        
        for (i = 0; i < 4; i++) {
            if (A[i][i] == 0.0f) {
                for (j = i + 1; j < 4; j++) { // Look for non-zero element in column
                    if (A[i][j] != 0.0f) {
                        break;
                    }
                }
                if (j != 4) { // Swap rows.
                    for (k = 0; k < 4; k++) { 
                        swap(A[k][i], A[k][j]);
                        swap(I[k][i], I[k][j]);
                    }
                }
                else return I;
            }
            
            t = 1.0f / A[i][i];
            for (k = 0; k < 4; k++) {
                A[k][i] *= t;
                I[k][i] *= t;
            }
            for (j = 0; j < 4; j++) {
                if (j != i) {
                    t = A[i][j];
                    for (k = 0; k < 4; k++) {
                        A[k][j] -= A[k][i]*t;
                        I[k][j] -= I[k][i]*t;
                    }
                }
            }
        }
        return I;
    }
    
    static MatrixType rotatex(T x)
    {
        MatrixType R;
        T ct = cos(x);
        T st = sin(x);
        R.m22 = ct;
        R.m23 =-st;
        R.m32 = st;
        R.m33 = ct; 
        return R;
    }
    static MatrixType rotatey(T x)
    {
        MatrixType R;
        T ct = cos(x);
        T st = sin(x);
        R.m11 = ct;
        R.m13 = st;
        R.m31 =-st;
        R.m33 = ct;
        return R;
    }
    static MatrixType rotatez(T x)
    {
        MatrixType R;
        T ct = cos(x);
        T st = sin(x);
        R.m11 = ct;
        R.m12 =-st;
        R.m21 = st;
        R.m22 = ct;
        return R;
    }

    static MatrixType rotate(const PointType<T>& axis, T angle)
    {
        PointType<T> a = axis.normal();
        T c = cos(angle);
        T s = sin(angle);

        MatrixType R;
        R.m11 = a.x * a.x + (1 - a.x * a.x) * c;
        R.m12 = a.x * a.y + (1 - c) - a.z * s;
        R.m13 = a.x * a.z + (1 - c) - a.y * s;
        R.m14 = 0;
        R.m21 = a.x * a.y + (1 - c) + a.z * s;
        R.m22 = a.y * a.y + (1 - a.y * a.y) * c;
        R.m23 = a.y * a.z + (1 - c) + a.x * s;
        R.m24 = 0;
        R.m31 = a.x * a.z + (1 - c) + a.y * s;
        R.m32 = a.y * a.z + (1 - c) + a.x * s;
        R.m33 = a.z * a.z + (1 - a.z * a.z) * c;
        R.m34 = 0;
        R.m41 = 0;
        R.m42 = 0;
        R.m43 = 0;
        R.m44 = 1;
        return R;
    }

    static MatrixType translate(const PointType<T> &pos)
    {
        MatrixType R;

        R.m41 = pos.x;
        R.m42 = pos.y;
        R.m43 = pos.z;

        return R;
    }
    
    
    static MatrixType lookat(const PointType<T> &eye,
                             const PointType<T> &dir,
                             const PointType<T> &up)
    {
        MatrixType R;

        PointType<T> d, u, v;
        d = dir.normal();
        u =  up.normal();
        v = d^u;
        u = v^d;

        R.m11 = v.m1;
        R.m21 = v.m2;
        R.m31 = v.m3;
        R.m41 = v.m4;

        R.m12 = u.m1;
        R.m22 = u.m2;
        R.m32 = u.m3;
        R.m42 = u.m4;

        R.m13 =-d.m1;
        R.m23 =-d.m2;
        R.m33 =-d.m3;
        R.m43 =-d.m4;

        return R * translate(-eye);
    }




    const char* str() const 
    {
        #pragma warning(disable : 4996)
        static char s[512];
		char* q = s;
    	for (int i=0;i<4;i++)
    		q += sprintf(q, "%6.2f %6.2f %6.2f %6.2f\n", 
	    			static_cast<float>(A[i][0]),
    				static_cast<float>(A[i][1]),
	    			static_cast<float>(A[i][2]),
	    			static_cast<float>(A[i][3]));
    		
    	return s;
    	
    }

};


#endif
