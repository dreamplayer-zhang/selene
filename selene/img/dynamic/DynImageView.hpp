// This file is part of the `Selene` library.
// Copyright 2017-2018 Michael Hofmann (https://github.com/kmhofmann).
// Distributed under MIT license. See accompanying LICENSE file in the top-level directory.

#ifndef SELENE_IMG_DYNAMIC_DYN_IMAGE_VIEW_HPP
#define SELENE_IMG_DYNAMIC_DYN_IMAGE_VIEW_HPP

/// @file

#include <selene/img/dynamic/DynImageIterators.hpp>
#include <selene/img/dynamic/UntypedLayout.hpp>

#include <type_traits>

namespace sln {

template <ImageModifiability modifiability_ = ImageModifiability::Constant>
class DynImageView
{
public:
  using DataPtrType = typename DataPtr<modifiability_>::Type;

  template <typename PixelType> using iterator = DynImageRowIterator<PixelType, modifiability_>;  ///< The iterator type.
  template <typename PixelType> using const_iterator = ConstDynImageRowIterator<PixelType, modifiability_>;  ///< The const_iterator type.

  constexpr static bool is_view = true;
  constexpr static bool is_owning = false;
  constexpr static bool is_modifiable = (modifiability_ == ImageModifiability::Mutable);
  constexpr static ImageModifiability modifiability() { return modifiability_; }

  DynImageView(DataPtr<modifiability_> ptr, UntypedLayout layout)
      : ptr_(ptr), layout_(layout)
  { }

  const UntypedLayout& layout() const noexcept { return layout_; }

  PixelLength width() const noexcept { return layout_.width; }
  PixelLength height() const noexcept { return layout_.height; }
  std::int16_t nr_channels() const noexcept { return layout_.nr_channels; }
  std::int16_t nr_bytes_per_channel() const noexcept { return layout_.nr_bytes_per_channel; }
  Stride stride_bytes() const noexcept { return layout_.stride_bytes; }
  std::ptrdiff_t row_bytes() const noexcept { return layout_.row_bytes(); }
  std::ptrdiff_t total_bytes() const noexcept { return layout_.total_bytes(); }
  PixelFormat pixel_format() const noexcept { return semantics_.pixel_format; }
  SampleFormat sample_format() const noexcept { return semantics_.sample_format; }
  bool is_packed() const noexcept { return layout_.is_packed(); }

  bool is_empty() const noexcept { return ptr_.data() == nullptr || layout_.width == 0 || layout_.height == 0; }
  bool is_valid() const noexcept { return !is_empty(); }

  template <typename PixelType> iterator<PixelType>       begin() noexcept        { return DynImageRowIterator<PixelType, modifiability_>(DynImageRow<PixelType, modifiability_>(this, 0_idx)); }
  template <typename PixelType> const_iterator<PixelType> begin() const noexcept  { return ConstDynImageRowIterator<PixelType, modifiability_>(ConstDynImageRow<PixelType, modifiability_>(this, 0_idx)); }
  template <typename PixelType> const_iterator<PixelType> cbegin() const noexcept { return ConstDynImageRowIterator<PixelType, modifiability_>(ConstDynImageRow<PixelType, modifiability_>(this, 0_idx)); }

  template <typename PixelType> iterator<PixelType>       end() noexcept        { return DynImageRowIterator<PixelType, modifiability_>(DynImageRow<PixelType, modifiability_>(this, PixelIndex{this->height()})); }
  template <typename PixelType> const_iterator<PixelType> end() const noexcept  { return ConstDynImageRowIterator<PixelType, modifiability_>(ConstDynImageRow<PixelType, modifiability_>(this, PixelIndex{this->height()})); }
  template <typename PixelType> const_iterator<PixelType> cend() const noexcept { return ConstDynImageRowIterator<PixelType, modifiability_>(ConstDynImageRow<PixelType, modifiability_>(this, PixelIndex{this->height()})); }

  DataPtrType byte_ptr() noexcept             { return ptr_.data(); }
  const DataPtrType byte_ptr() const noexcept { return ptr_.data(); }

  DataPtrType byte_ptr(PixelIndex y) noexcept             { return ptr_.data() + this->compute_data_offset(y); }
  const DataPtrType byte_ptr(PixelIndex y) const noexcept { return ptr_.data() + this->compute_data_offset(y); }

  DataPtrType byte_ptr(PixelIndex x, PixelIndex y) noexcept             { return ptr_.data() + this->compute_data_offset(x, y); }
  const DataPtrType byte_ptr(PixelIndex x, PixelIndex y) const noexcept { return ptr_.data() + this->compute_data_offset(x, y); }

  template <typename PixelType, typename T = void, typename = std::enable_if_t<modifiability_ == ImageModifiability::Mutable, T>>
  PixelType* data() noexcept             { return reinterpret_cast<PixelType*>(this->byte_ptr()); }

  template <typename PixelType>
  const PixelType* data() const noexcept { return reinterpret_cast<const PixelType*>(this->byte_ptr()); }

  template <typename PixelType, typename T = void, typename = std::enable_if_t<modifiability_ == ImageModifiability::Mutable, T>>
  PixelType* data(PixelIndex y) noexcept             { return reinterpret_cast<PixelType*>(this->byte_ptr(y)); }

  template <typename PixelType>
  const PixelType* data(PixelIndex y) const noexcept { return reinterpret_cast<const PixelType*>(this->byte_ptr(y)); }

  template <typename PixelType, typename T = void, typename = std::enable_if_t<modifiability_ == ImageModifiability::Mutable, T>>
  PixelType* data_row_end(PixelIndex y) noexcept             { return reinterpret_cast<PixelType*>(this->byte_ptr(y) + layout_.nr_bytes_per_pixel() * layout_.width); }

  template <typename PixelType>
  const PixelType* data_row_end(PixelIndex y) const noexcept { return reinterpret_cast<const PixelType*>(this->byte_ptr(y) + layout_.nr_bytes_per_pixel() * layout_.width); }

  template <typename PixelType, typename T = void, typename = std::enable_if_t<modifiability_ == ImageModifiability::Mutable, T>>
  PixelType* data(PixelIndex x, PixelIndex y) noexcept             { return reinterpret_cast<PixelType*>(this->byte_ptr(x, y)); }

  template <typename PixelType>
  const PixelType* data(PixelIndex x, PixelIndex y) const noexcept { return reinterpret_cast<const PixelType*>(this->byte_ptr(x, y)); }

  template <typename PixelType, typename T = void, typename = std::enable_if_t<modifiability_ == ImageModifiability::Mutable, T>>
  PixelType& pixel(PixelIndex x, PixelIndex y) noexcept             { return *data<PixelType>(x, y); }

  template <typename PixelType>
  const PixelType& pixel(PixelIndex x, PixelIndex y) const noexcept { return *data<PixelType>(x, y); }

  DynImageView<ImageModifiability::Mutable>& view() noexcept { return *this; }
  DynImageView<ImageModifiability::Constant> view() const noexcept { return DynImageView<ImageModifiability::Constant>{this->byte_ptr(), this->layout()}; }  // TODO: optimize

  void clear()
  {
    ptr_ = DataPtr<modifiability_>{};
    layout_ = UntypedLayout{};
    semantics_ = UntypedImageSemantics{};
  }

private:
  DataPtr<modifiability_> ptr_;
  UntypedLayout layout_;
  UntypedImageSemantics semantics_;

  sln::Bytes compute_data_offset(PixelIndex y) const noexcept
  {
    return sln::Bytes{layout_.stride_bytes * y};
  }

  sln::Bytes compute_data_offset(PixelIndex x, PixelIndex y) const noexcept
  {
    return sln::Bytes{layout_.stride_bytes * y + layout_.nr_bytes_per_pixel() * x};
  }
};

}  // namespace sln

#endif  // SELENE_IMG_DYNAMIC_DYN_IMAGE_VIEW_HPP
