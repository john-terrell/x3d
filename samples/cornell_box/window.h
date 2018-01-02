#ifndef test1__window_h
#define test1__window_h

class window
{
    int glut_id;
    struct window_data *data;
    
public:
	float _rotation_angle;
    
public:
    window( int x, int y, unsigned width, unsigned height );
    virtual ~window();
    
    virtual void refresh();
    virtual void idle();
    virtual void timer( int );
    virtual void keyboard( unsigned char key, int x, int y );
};

#endif // test1__window_h
