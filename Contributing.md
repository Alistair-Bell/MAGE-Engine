# Mage Engine contributing guide

If you want to contribute to the development to the project then here are some rules to follow

### Naming schemes
- All mage related methods, typedef's and structs must begin with `mage`
- All macros delcared must be formatted like `MAGE_FOO_BAR`
- Enums should be typedefed and be formatted as 
    ```c
    typedef enum MAGE_SOME_ENUM 
    { 
        MAGE_SOME_ENUM_VALUE  = 1
    } mageSomeEnum
    ```
- Don't typedef structs, unless it is required I prefer knowing that my type is a struct
- Methods naming scheme goes mageSubjectAction e.g. `mageVector3Create`
- Avoid abbreviations `mageShaderSrc` ❌ `mageShaderSource` ✅
- Struct values should avoid using the struct name
```c
    struct mageFoo
    {
        uint32_t FooBar; ❌
    };
    struct mageFoo
    {
        uint32_t Bar; ✅
    }
```

### General programming
- Keep all non static or source specific definitions in `mageAPI.h` 
- All platform specific should be located within `Mage/Source/YourPlatform/mageYourSource`
- All C files begin with `mageYourSource`
- Avoid creating new header files, the project should use a single header

### General philosophy
- The code should be simple yet effective, doing what it should with speed
- Code should be modular and abstract enough to allow for long periods of backwards compatability with new features
- The code should be a small as possible along with it's assets, no-one likes large downloads or long compile times
