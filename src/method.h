/**
 *  Method.h
 *
 *  Internal class that represents a native class method, that can be called
 *  from PHP scripts.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Class definition
 */
class Method : public Callable
{
public:
    /**
     *  Constructor 
     * 
     *  @param  name            Method name
     *  @param  callback        Native callback
     *  @param  flags           Access flags
     *  @param  args            Argument description
     */
    Method(const char *name, method_callback_0 callback, int flags, const Arguments &args) : Callable(name, args), _type(0), _flags(flags) { _callback.m0 = callback; }
    Method(const char *name, method_callback_1 callback, int flags, const Arguments &args) : Callable(name, args), _type(1), _flags(flags) { _callback.m1 = callback; }
    Method(const char *name, method_callback_2 callback, int flags, const Arguments &args) : Callable(name, args), _type(2), _flags(flags) { _callback.m2 = callback; }
    Method(const char *name, method_callback_3 callback, int flags, const Arguments &args) : Callable(name, args), _type(3), _flags(flags) { _callback.m3 = callback; }
    Method(const char *name,                             int flags, const Arguments &args) : Callable(name, args), _type(4), _flags(flags) { _callback.m0 = nullptr;  }

    /**
     *  Copy and move constructors
     *  @param  that
     */
    Method(const Method &that) : Callable(that), _type(that._type), _flags(that._flags), _callback(that._callback) {}
    Method(Method &&that) : Callable(std::move(that)), _type(that._type), _flags(that._flags), _callback(that._callback) {}

    /**
     *  Destructor
     *  @param  type
     *  @param  callback
     */
    virtual ~Method() {}

    /**
     *  Internal method to fill a function entry
     *  @param  zend_function_entry
     *  @param  classname
     */
    void initialize(struct _zend_function_entry *entry, const std::string &classname)
    {
        // fix the flags, if neither public, private and protected is set, we use public,
        // (this solves php warnings if only "final" or only "abstract" is set
        if ((_flags & (Public|Private|Protected)) == 0) _flags |= Public;
        
        // call base
        Callable::initialize(entry, classname.c_str(), _flags);
    }

    /**
     *  Invoke the method
     *  @param  parameters
     *  @return Value
     */
    virtual Value invoke(Parameters &parameters) override
    {
        // the object to call a method on
        Base *base = parameters.object();
        
        // find out which method to call, and call it
        switch (_type) {
        case 0:     (base->*_callback.m0)(); return Value();
        case 1:     (base->*_callback.m1)(parameters); return Value();
        case 2:     return (base->*_callback.m2)();
        case 3:     return (base->*_callback.m3)(parameters);
        default:    return Value();
        }
    }


private:
    /**
     *  Callback type
     *  @var int
     */
    int _type;

    /**
     *  Access flags (protected, public, abstract, final, private, etc)
     *  @var int
     */
    int _flags;
    
    /**
     *  The actual callback
     *  @var void*
     */
    union {
         method_callback_0 m0;
         method_callback_1 m1;
         method_callback_2 m2;
         method_callback_3 m3;
    } _callback;
};

/**
 *  End of namespace
 */
}
