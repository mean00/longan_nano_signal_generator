#pragma once

/**
 */
class Action
{
public:
    virtual void turnLeft()=0;
    virtual void turnRight()=0;
    virtual void shortPress()=0;
    virtual void setParent(Action *a) {}
};
/**
 */
class TopAction : public Action
{
public:
    TopAction()
    {
        index=0;
        max=0;
        selection=-1;
    }
    virtual void turnLeft()
    {
        index=(index+max-1)%max;
    }
    virtual void turnRight()
    {
        index=(index+1)%max;
    }
    virtual void shortPress()
    {
        selection=index;
    };
    int getValue()
    {
        return index;
    }
    void addAction(Action *a)
    {
        actions[max]=a;
        a->setParent(this);
        max++;
    }
    Action *getCurrent()
    {
        if(selection==-1) 
            return this;
        return actions[selection];
    }
    int  getSelection()
    {
        return selection;
    }
    void exitSelection()
    {
        selection=-1;
    }

protected:
    int     index;
    int     max;
    Action  *actions[10];
    int     selection;
};
/**
 * 
 */
class DigitAction : public Action
{
public:
    DigitAction(int mx)
    {
        index=0;
        _parent=NULL;
        _max=mx;
    }
    int getValue()
    {
        return index;
    }
    void setValue(int v)
    {
        index=v;
    }
    virtual void turnLeft()
    {
        index=(index+_max-1)%_max;
    }
    virtual void turnRight()
    {
        index=(index+1)%_max;
    }
    virtual void shortPress()
    {
        ((TopAction *)_parent)->exitSelection(); // Ugly but simple
    };
    
    virtual void setParent(Action *a) {_parent=a;}
protected:
    int     index;
    Action *_parent;
    int     _max;
    
};

/**
 * 
 */
class WaveAction : public Action
{
public:
    WaveAction(int mx, SignalGenerator::WaveForm *wv)
    {
        _parent=NULL;
        _max=mx;
        _wav=wv;
    }
    virtual void turnLeft()
    {
        int index=(int)*_wav;
        index=(index+_max-1)%_max;
        *_wav=( SignalGenerator::WaveForm )index;
    }
    virtual void turnRight()
    {
        int index=(int)*_wav;
        index=(index+1)%_max;
        *_wav=( SignalGenerator::WaveForm )index;
    }
    virtual void shortPress()
    {
        ((TopAction *)_parent)->exitSelection(); // Ugly but simple
    };
    
    virtual void setParent(Action *a) {_parent=a;}
protected:
    int     index;
    Action *_parent;
    int     _max;
     SignalGenerator::WaveForm *_wav;
    
};