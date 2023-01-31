#pragma once

#define InternDerivedComponent(type) \
protected:                                                              \
void DestroyData(RawByte* address) override                             \
{                                                                       \
reinterpret_cast<##type*>(address)->~##type();                       \
}                                                                       \
\
void MoveData(RawByte* source, RawByte* destination) const override     \
{                                                                       \
new (destination) (##type){ std::move(*reinterpret_cast<##type*>(source)) };    \
reinterpret_cast<##type*>(source)->~##type();                                 \
}                                                                       \
SizeType GetSize() const override                                       \
{                                                                       \
return sizeof(##type);                                                \
}                                                                       \


#define SFML_SUPPORT 1
#define USE_SFML_WINDOW 1
#define USE_SFML_RENDERER 1
#define RELEASE_WITH_LOGINFO 1
#define USE_IMGUI 1
