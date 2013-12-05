#ifndef TIME_H
#define TIME_H

class Timing
{
public:
    //! Time between two frames in ms
    /*!
    */
    static const int fps_delta_time;
    //! Number of frames per second
    /*!
    */
    static const int fps;
    static const int ups;
    static const int ups_delta_time;
private:
    virtual void abstract()=0;
};

#endif // TIME_H
