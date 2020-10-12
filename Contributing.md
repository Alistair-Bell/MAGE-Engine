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

# Github issues guides
- If you come across an issue (bug, crash, etc) then please report the issue to the [github issues](https://github.com/Alistair-Bell/Mage-Engine/issues)
- Reports are essential to maintaining a well developed project, even if you are unable to personally fixed any reports will be apreciated
- To ensure that the issue gets dealt with in a quick method and effect manner formatting it in a uniform way will greatly help the developers working to fix bugs and any unexpected issues
  
### Required fields
- When filing issues make sure to provide details about your environment, and information on the issue at hand
    > 1. What platform was this found on? (linux, windows, mac or other)
    > 2. What hardware is this running on? (cpu and gpu)
    > 3. What version of the engine was this found on?
    > 4. Small description of the issue
    > 6. What did you expect to see?
    > 6. What did you see instead?
    > 7. Is this bug reproducable? If so provide how do you produce it?
### Extra information
- Any extra unrequired information on issues are welcome and encouraged this may included and not limited to
    > 1. Solutions to issues (theoretical or actual code)
    > 2. Theories on why the issue occurs
    > 3. Specific build settings (compilers used, renderering backends, etc)
- The more information provided the quicker problems can be solved
- See [model issue](https://github.com/Alistair-Bell/Mage-Engine/issues/17) for a template