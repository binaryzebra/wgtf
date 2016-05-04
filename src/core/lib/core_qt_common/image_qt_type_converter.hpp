#ifndef IMAGE_QT_TYPE_CONVERTER_HPP
#define IMAGE_QT_TYPE_CONVERTER_HPP

#include "i_qt_type_converter.hpp"

class ImageQtTypeConverter : public IQtTypeConverter
{
public:
	bool toVariant( const QVariant & qVariant, Variant & o_variant ) const override;
	bool toQVariant( const Variant & variant, QVariant & o_qVariant ) const override;
};

#endif // IMAGE_QT_TYPE_CONVERTER_HPP