#ifndef MATRIX_H
#define MATRIX_H

#include "iostream"
#include <vector>

using namespace std;

 template<class T> class Matrix;
 template<class T>  ostream& operator<<   (ostream& out, const Matrix<T>& w);

 template<class T>
class Matrix
 {
private :
        int rows, cols;// 矩阵维数
        vector< vector<T> > _data;

        uint rows_in; //Iterators for m and n ( Operator << )
        uint cols_in; //Starts at 0.
        uint rows_out; //Iterators for m and n ( Operator >> )
        uint cols_out; //Starts at 0.
        char _type;
        bool _error;
        bool _error_verbose;
        double _determinant;
        bool _det_up_to_date;

        double _det( Matrix<T>  in ); //Auxiliary recursive function to calculate determinant.

public :
        Matrix();
        Matrix(int r , int c );
        Matrix(const Matrix<T>& m); //复制构造函数
        ~Matrix() {}

        void print();
        void Write_Matrix();
        int Rows() const {return rows;}
        int Columns() const {return cols;}
        T& operator()(int i, int j) const;

        operator double();

        Matrix<T>& operator << ( T input );
        Matrix<T>&  operator = (const Matrix<T>& m);
        Matrix<T> operator+() const; // 一元加法
        Matrix<T> operator+(const Matrix<T>& m) const;
        Matrix<T> operator-() const; //一元减法
        Matrix<T> operator-(const Matrix<T>& m) const;

        Matrix<T> operator*(double num) const;

        Matrix<T> operator*(const Matrix<T>& m) const;

        Matrix<T>& operator+=(const T& x);

        double det();
        Matrix<double>  inv();        //Inverse

        friend ostream& operator<<(ostream& out, const Matrix<T>& w);

};



template <class T>
Matrix<T>::Matrix()
{
    rows = 0;
    cols = 0;
    _error =0;
    rows_in=0;
    cols_in=0;
    rows_out=0;
    cols_out=0;
}

template <class T>
Matrix<T>::Matrix(int r , int c)
{
    if(c > 0 || r >0)
    {
        rows = r;
        cols = c;
        _error =0;
        rows_in=0;
        cols_in=0;
        rows_out=0;
        cols_out=0;
         _data.resize(r);
        for(int i = 0 ; i < r ; i++)
        {
              _data[i].resize(c);//现在你可以和使用数组一样使用这个vector

        }
        for (int i = 0; i < rows  ; i ++)
        {
            for(int j = 0 ; j < cols ; j++)
            {
                _data[i][j] = T(0.0);
            }
        }
    }
}

template<class T>
  Matrix<T>::Matrix(const Matrix<T>& m) //复制构造函数
  {
    rows = m.rows;
    cols = m.cols;
    _data = m._data;
  }

template<class T>
void Matrix<T>::Write_Matrix()
{
    for (int i = 0; i < rows  ; i ++)
        for(int j = 0 ; j < cols ; j++)
    {
        cin>>_data[i][j];
    }
}
// <<  insert input to the matrix
template<class T>
Matrix<T>&  Matrix<T>::operator << ( T input )
{
        //The IO iterators can be an issue here. Currently, resize() handles that.
      if( rows == 0 || cols == 0 || _error )
        {
        cout<<"Attempting to input into an empty matrix." <<endl;
        _error = 0;
      }
        _data[rows_in][cols_in] = input;
        cols_in++;
      if( cols_in == cols )
      {
        cols_in = 0;
        rows_in++;
      }
      if( rows_in == rows )
      {
        rows_in = 0;
      }
      _det_up_to_date = 0;
      return *this;
}

template <class T>
void Matrix<T>::print()
{
  for( uint i = 0; i < rows; i++ )
    {
        for( uint j = 0; j < cols; j++ )
        {
          cout << _data[i][j] << " ";
        }
    cout << endl;
  }
}


template<class T>
T& Matrix<T>::operator()(int i, int j) const
{   // 返回一个指向元素( i , j )的引用
    return _data[i][j];
}

template <class T>
Matrix<T>&  Matrix<T> ::operator =  (const Matrix<T>& m)
{
    if( this == & m)
        return *this;

    rows = m.rows;
    cols = m.cols;
    _data = m._data;
}


template <class T>
 Matrix<T>  Matrix<T>:: operator * (double num) const
 {
    Matrix<T> result_multiplicarion(rows,cols);
    for (int i = 0; i < rows ; i++)
         for(int j = 0 ; j < cols ; j++)
    {
        result_multiplicarion._data[i][j] =  _data[i] * num;
    }
    return result_multiplicarion;
 }

template <class T>
 Matrix<T>  Matrix<T> ::operator*(const Matrix<T>& m) const
 {
     if(cols == m.rows)
     {
         double sum = 0.0;
         Matrix<T> w(rows , m.cols);
         for(int i = 0;i < rows ;i++)
         {
             for(int j = 0 ; j < m.cols ; j++)
             {
                 for(int k = 0; k < cols ; k++)
                 {
                      sum += (*this)(i+1 , k+1) * m(k+1,j+1);
                 }
                w(i + 1, j + 1) = sum;
                sum = 0.0;
             }

          }
        return w;
     }
 }

template <class T>
Matrix<T> Matrix<T> ::operator+(const Matrix<T>& m) const
{
        // 返回(*this) - m.
    if (rows==m.rows&&cols== m.cols)
    {
// 创建结果矩阵w
        Matrix<T> w(rows,cols);

        for (int i = 0; i < rows ; i++)
            for(int j = 0; j < cols ; j++)
                 w._data[i][j] = _data[i][j]+m._data[i][j];
        return w;
    }
}

template <class T>
Matrix<T> Matrix<T> ::operator - (const Matrix<T>& m) const
{
        // 返回(*this) - m.
    if (rows==m.rows&&cols== m.cols)
    {
// 创建结果矩阵w
        Matrix<T>  w(rows,cols);
        for (int i = 0; i < rows ; i++)
            for(int j = 0; j < cols ; j++)
                 w._data[i][j] = _data[i][j] - m._data[i][j];
        return w;
    }
}



template <class T>
ostream& operator<<(ostream& out, const Matrix<T>& w)
{
    for(int i=0;i<w.rows;i++)
    {
         for(int j = 0; j < w.cols ; j++)
            {
                out<<w._data[i][j]<<" ";
            }
        out<<endl;
    }

    return out;
}

template <class T>
double Matrix<T>::det()
{
    if( _det_up_to_date )
         return _determinant;
    else
    {
        if( rows != cols )
        {
          return 0;
        }
        _det_up_to_date = true;
        _determinant = _det( *this );
         return _determinant;
    }
}

template <class T>
double  Matrix<T>::_det( Matrix<T>  in )
{
      uint rows = in.rows;
      uint cols = in.cols;
      if( rows == 2 )
      {
        return in._data[0][0]*in._data[1][1] - in._data[1][0]*in._data[0][1];
      }
      if( rows == 1 )
      {
        return in._data[0][0];
      }
      double sum = 0.0;
      int sign = 1;
            for( uint iter = 0; iter < in.rows; iter++ )
              {
               Matrix<T> out( rows - 1, cols - 1 );
                for( uint i = 1; i < in.rows; i++ )
                {
                     for( uint j = 0; j < in.cols; j++ )
                    {
                        if( j != iter )
                        {
                            out << in._data[i][j];
                        }
                    }
                }
                sum = sum + in._data[0][iter] * sign * _det( out );
                sign = sign * -1;
            }
  return sum;
}

/***********************************************************************************

************************************************************************************/
template<class T>
 Matrix<double>  Matrix<T> ::inv()
{
	if(rows != cols)
	{
		cout<<"The matrix is not SQUARE"<<endl;
		_error = 1;
	}
	else
	{
		int   M = rows;
		int   N = 2*M;
		int i , j , k;
		double b[M][N];

		Matrix<double> result(M,M);

		for(i=0;i<M;i++)
		{
			for(j=0;j<M;j++)
			{
				b[i][j] = double( _data[i][j] );
		        	}
		}
		/*****************扩展矩阵***********************/
	    	for(i=0;i<M;i++)
		{
			for(j=M;j<N;j++)
			{
			            if(i==(j-M))
			            {
			                b[i][j]=1;
			            }
				else
			            {
			                b[i][j]=0;
			            }
	    		}
	 	}
	    /*****************扩展矩阵***********************/

	    /*****************求逆模块***********************/
		for(i=0;i<M;i++)
		{
			if(b[i][i]==0)
			{
				for(k=i;k<M;k++)
				{
					if(b[k][k]!=0)
					{
						for(int j=0;j<N;j++)
						{
					                        double temp;
					                        temp=b[i][j];
					                        b[i][j]=b[k][j];
					                        b[k][j]=temp;
		           				}
		                   		 	break;
		                		}
	          			}

		            		if(k==M)
		           		{
		                		cout<<"该矩阵不可逆！"<<endl;
		           		}

	       		}

		       	for(j=N-1;j>=i;j--)
		      	{
				b[i][j]/=b[i][i];
			}

			for(k=0;k<M;k++)
			{
				if(k!=i)
				{
					double temp=b[k][i];
					for(j=0;j<N;j++)
					{
						b[k][j]-=temp*b[i][j];
					}
				}
			}
		}
	    /*****************求逆模块***********************/

	    /*****************导出结果***********************/
	    for(i=0;i<M;i++)
	    {
	        for(j=M;j<N;j++)
	        {
	            result. _data[i][j-M] = b[i][j];
	      //      cout<<b[i][j]<<" ";
	        }
	    //    cout<<endl;
	    }
	    /*****************导出结果***********************/
	return result;
	}

}

#endif // MATRIX_H_INCLUDED
