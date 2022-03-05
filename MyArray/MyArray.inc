#ifndef MY_ARRAY_H__
#define MY_ARRAY_H__


// use before include typedef type MY_ARRAY_TYPE

struct MyArray
{
    size_t ArrayCapacity;
    size_t ArrayCurSize;
    size_t ElementSize;
    MY_ARRAY_TYPE*  Elements;
};


static int MyArrayAllocValue(MyArray* my_array, const size_t capacity);

static int MyArrayAddValue(MyArray* my_array, MY_ARRAY_TYPE value);


static int MyArrayAddValue(MyArray* my_array, MY_ARRAY_TYPE value)
{
    assert(my_array);
    assert(value);

    int status = 0;

    if (my_array->ArrayCurSize >= my_array->ArrayCapacity)
    {

        if ((status = MyArrayAllocValue(my_array, (size_t)(my_array->ArrayCapacity * TokenScale))) != 0) // #err
            return 0; // #err
    }

    my_array->Elements[my_array->ArrayCurSize++] = value;

    return 0; // #err
}

static int MyArrayAllocValue(MyArray* my_array, const size_t capacity)
{
    assert(my_array);

    MY_ARRAY_TYPE* elements = (MY_ARRAY_TYPE*)realloc(my_array->Elements, my_array->ElementSize * capacity);

    if (!elements)
    {
        // #log
        return 0; // #err
    }

    my_array->Elements = elements;
    my_array->ArrayCapacity = capacity;

    return 0; // #err
}


#endif