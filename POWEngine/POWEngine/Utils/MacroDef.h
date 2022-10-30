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
