
template <class Destructor>
CompositeObject<Destructor>::~CompositeObject<Destructor>()
{
    if(isSelected())
        for (auto ptr : _list) {

            ptr->silence();
            _destructor(ptr);
        }
    notify();
}


template <class Destructor>
void CompositeObject<Destructor>::move(float dx, float dy)
{
    for (auto& ptr : _list) {

        ptr->silence();
        ptr->setCoord(ptr->getX() + dx, ptr->getY() + dy);
        ptr->sound();
    }
    notify();
}


template <class Destructor>
void CompositeObject<Destructor>::select()
{
    for (auto& ptr : _list) {

        ptr->silence();
        ptr->select();
        ptr->sound();
    }
    CanvasObject::select();
}


template <class Destructor>
void CompositeObject<Destructor>::deselect()
{
     for (auto& ptr : _list) {

        ptr->silence();
        ptr->deselect();
        ptr->sound();
    }
    CanvasObject::deselect();
}
