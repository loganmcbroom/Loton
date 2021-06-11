#pragma once

#include <nodes/TypeConverter>

#define Loton_FUNCTION_CONVERTER_MACRO( T1, T2 ) struct T1 ## To ## T2 ## Converter : public QtNodes::TypeConverter\
{\
	virtual void operator()( std::shared_ptr<QtNodes::NodeData> data ) override;\
	virtual std::shared_ptr<QtNodes::TypeConverter> createNew() override \
		{ return std::make_shared<T1 ## To ## T2 ## Converter>(); }\
};

Loton_FUNCTION_CONVERTER_MACRO( Number, Func1x1 )
Loton_FUNCTION_CONVERTER_MACRO( Number, Func2x1 )
Loton_FUNCTION_CONVERTER_MACRO( Number, Func2x2 )

Loton_FUNCTION_CONVERTER_MACRO( Func1x1, Number )
Loton_FUNCTION_CONVERTER_MACRO( Func1x1, Func2x1 )
Loton_FUNCTION_CONVERTER_MACRO( Func1x1, Func2x2 )
Loton_FUNCTION_CONVERTER_MACRO( Func1x1, Interpolator )

Loton_FUNCTION_CONVERTER_MACRO( Func2x1, Number )
Loton_FUNCTION_CONVERTER_MACRO( Func2x1, Func1x1 )
Loton_FUNCTION_CONVERTER_MACRO( Func2x1, Func2x2 )
