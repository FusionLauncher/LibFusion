#ifndef FEXCEPTION_H
#define FEXCEPTION_H


#include <QString>
//#include <exception>

class FException // :public std::exception
{
public:
    FException(QString msg) {Message = msg;}
   /* virtual const char* what() const throw()
    {
        return "An error occured!";
    }
*/
    QString what() {return "that."; }
    QString Message;
};

#endif // FEXCEPTION_H
