

#pragma once



class FSMWeakReference {



private:
    bool mValidStrongReference_, mValidWeakReference_; 
    
    
    //And then this class isn't used as a std::unique_ptr or a std::shared_ptr 
    //but just a normal pointer that is allocated manually each time a new weakReference
    //object is neeeded. Then when deciding whether to destruct it or not, just check to see if
    //the other sides flag has been set to determine whether to free the data or not... OR IDK...


};

