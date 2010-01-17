/*
4D Homogeneous Point Class
Tom Flanagan
*/

#ifndef POINT_HEADER
#define POINT_HEADER

#include <sstream>

template <typename T=float>
struct PointType
{
    #pragma warning(disable : 4201) // anonymous union/struct
    union
    {
        T A[4];
        struct { T a1,a2,a3,a4;};
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        struct { T m1,m2,m3,m4;};
    };

    static PointType angle(T t, T p, T d=0)
    {
        return PointType(
            sin(t) * cos(p),
            sin(t) * sin(p),
            cos(t), d);
                            
    }

    PointType(T a1=0, T a2=0, T a3=0, T a4=1)
    {   
        A[0] = a1;
        A[1] = a2;
        A[2] = a3;
        A[3] = a4;
    }
    
    template <typename U>
    PointType(U v[3])
    {
        A[0] = static_cast<T>(v[0]);
        A[1] = static_cast<T>(v[1]);
        A[2] = static_cast<T>(v[2]);
        A[3] = 1;
    }

    template <typename U>
    PointType(U *v, int n)
    {
        A[0] = n>=0 ? static_cast<U>(v[0]) : 0;
        A[1] = n>=1 ? static_cast<U>(v[1]) : 0;
        A[2] = n>=2 ? static_cast<U>(v[2]) : 0;
        A[3] = n>=3 ? static_cast<U>(v[3]) : 1;
    }

    // T from U
    template <typename U>
    PointType(const PointType<U> &B)
    {  
        A[0] = static_cast<U>(B[0]); 
        A[1] = static_cast<U>(B[1]); 
        A[2] = static_cast<U>(B[2]); 
        A[3] = static_cast<U>(B[3]); 
    }
    
    // U from T  (do I need both of these?)
    template <typename U>
    operator PointType<U>() const
    {   
        PointType<U> B;
        B[0] = static_cast<U>(A[0]); 
        B[1] = static_cast<U>(A[1]); 
        B[2] = static_cast<U>(A[2]); 
        B[3] = static_cast<U>(A[3]); 
        return B;
    }

	operator T*() { return A; }


    // A[x]
    template <typename U>
    inline const U& operator[](int i) const { return static_cast<U>(A[i]); }
    inline const T& operator[](int i) const { return                A[i] ; } // why isn't this handled by U?
    inline       T& operator[](int i)       { return                A[i]; }
    
   
    // A = B
    inline PointType& operator=(const PointType &B)
    {
        A[0] = B[0],
        A[1] = B[1];
        A[2] = B[2];
        A[3] = B[3];  
        return *this;
    }

    // A + B
    inline PointType operator+(const PointType &B) const
    {
        return PointType(
            A[0] + B[0],
            A[1] + B[1],
            A[2] + B[2],
            A[3] + B[3]
        );
    }

    // A += B
    inline PointType& operator+=(const PointType &B)
    {
        A[0] += B[0],
        A[1] += B[1];
        A[2] += B[2];
        A[3] += B[3];           
        return *this;
    }

    // A - B
    inline PointType operator-(const PointType &B) const
    {
        return PointType(
            A[0] - B[0],
            A[1] - B[1],
            A[2] - B[2],
            A[3] - B[3]
        );
    }

    // (-A)
    inline PointType operator-() const
    {
        return PointType(
            -A[0],
            -A[1],
            -A[2],
            -A[3]
        );
    }

    // A * f
    inline PointType operator*(T v) const
    {
        return PointType(
            A[0] * v,
            A[1] * v,
            A[2] * v,
            A[3] * v);
    }
    // A / f
    inline PointType operator/(T v)  const  {  return (*this) * (1/v);  }

    // A *= f
    inline PointType& operator*=(T v)
    {
        A[0] *= v;
        A[1] *= v;
        A[2] *= v;
        A[3] *= v;
        return *this;
    }

    // A % B (peicewise multiplication)
    inline PointType operator%(const PointType &B) const
    {
        return PointType(
            A[0] * B[0],
            A[1] * B[1],
            A[2] * B[2],
            A[3] * B[3]);              
    }

    inline PointType& operator%=(const PointType &B)
    {
        A[0] *= B[0];
        A[1] *= B[1];
        A[2] *= B[2];
        A[3] *= B[3];              
        return *this;
    }

    // A /= f
    inline PointType& operator/=(T v)    {  return (*this) *= (1/v);  }

    inline T abssq() const { return dot(*this);  }
    inline T abs()   const { return sqrt(abssq()); }
    
    inline T distsq(const PointType &B) const
    { 
    	return SQ(A[0]-B[0]) + SQ(A[1]-B[1]) + SQ(A[2]-B[2]);
   	}
   	inline T dist(const PointType &B) const { return sqrt(distsq(B)); }
    
    inline PointType  normal()     const { return (*this) /  abs(); }
    inline PointType& normalize()  const { return (*this) /= abs(); }


    inline T dot(const PointType &B) const
    { // Note that this does not include w*w
        T R = 0;
        R += A[0] * B[0];
        R += A[1] * B[1];
        R += A[2] * B[2];
        return R;
    }
    // A * B
    inline T operator*(const PointType &B) const { return dot(B); }

    inline PointType cross(const PointType &B) const
    { // Note that this does not include w, and always returns a vector
        return PointType(   
            y * B.z - z * B.y,
            z * B.x - x * B.z,
            x * B.y - y * B.x,
            0
        );
    }
    // A ^ B
    inline PointType operator^(const PointType &B) const { return cross(B); }

    inline T theta() const 
    {
        return acos(z / abs());
    }
    inline T phi() const
    {
        return atan2(y, x);
    }

    inline PointType project(const PointType &B) const
    { 
        const PointType &A = *this; 
        return A*((A*B)/(A*A)); 
    }
    
    const char* str() const
    {
        static char s[256];

        #pragma warning(disable : 4996)
        stringstream stream;
        stream << "<" << a1 << ", " << a2 << ", " << a3 << ", " << a4 << ">";
        // deprecated: sprintf(s, "<%6.2f, %6.2f, %6.2f, %6.2f>", (float)a1,(float)a2,(float)a3,(float)a4);
        s = stream.str().c_str();
        return s;
    }

};

template <typename T>
PointType<T> operator*(T f, const PointType<T> &A) { return A * f; }


// the default point
typedef PointType<float>  Point;

typedef PointType<float>  Pointf;
typedef PointType<double> Pointd;


Point ORIGIN;
Point X(1,0,0,0);
Point Y(0,1,0,0);
Point Z(0,0,1,0);


#endif
