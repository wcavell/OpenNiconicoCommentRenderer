#include "pch.h"
#include "renderer_window.h"

renderer_window::renderer_window( const std::shared_ptr<virtual_comment_server> cmtsvr )
	: cmtsvr_( cmtsvr )
	, renderer_( *reinterpret_cast<renderer_driver*>( &driver_ ) )
{
	set_text( L"nico renderer demo app" );
	set_height( 384 );
	set_width( 672 );

	cmtsvr_->add_handler(
		std::bind( std::mem_fn( &renderer_window::on_comment ), this, std::placeholders::_1 ),
		std::bind( std::mem_fn( &renderer_window::on_comment_mode_changed ), this, std::placeholders::_1 ) );
}

HRESULT renderer_window::initialize( HINSTANCE hinstance ) noexcept
{
	HRESULT hr = S_OK;

	hr = mnfx::directx_window::initialize( std::move( hinstance ), L"nico renderer demo app - renderer window" );
	if( FAILED( hr ) )
	{
		return hr;
	}

	return hr;
}

HRESULT renderer_window::initialize_resources() noexcept
{
	HRESULT hr = S_OK;

	hr = directx_window::initialize_resources();
	if( FAILED( hr ) )
	{
		return hr;
	}

	driver_.set_dwrite_factory( dwrite_factory().Get() );

	return hr;
}

HRESULT renderer_window::initialize_device_dependent_resources() noexcept
{
	HRESULT hr = S_OK;

	hr = directx_window::initialize_device_dependent_resources();
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = driver_.initialize( d2d_device_context().Get() );

	return hr;
}

HRESULT renderer_window::initialize_size_dependent_resources() noexcept
{
	HRESULT hr = S_OK;

	hr = directx_window::initialize_size_dependent_resources();
	if( FAILED( hr ) )
	{
		return hr;
	}

	const auto& dpi = 96.0f * physical_width() / 672.0f;
	d2d_device_context()->SetDpi( dpi, dpi );

	//const auto& bitmap_properties = D2D1::BitmapProperties1(
	//	D2D1_BITMAP_OPTIONS_TARGET,
	//	D2D1::PixelFormat( DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED ),
	//	dpi,
	//	dpi );

	//hr = d2d_device_context()->CreateBitmap(
	//	D2D1::SizeU( physical_width(), physical_height() ),
	//	nullptr,
	//	0,
	//	&bitmap_properties,
	//	back_buffer_bitmap_.GetAddressOf() );

	return hr;
}

HRESULT renderer_window::on_render() noexcept
{
	//d2d_device_context()->SetTarget( back_buffer_bitmap_.Get() );
	renderer_.render();

	//d2d_device_context()->SetTarget( d2d_bitmap().Get() );
	//d2d_device_context()->BeginDraw();
	//d2d_device_context()->DrawImage(
	//	back_buffer_bitmap_.Get(),
	//	D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
	//	D2D1_COMPOSITE_MODE_SOURCE_COPY );
	//d2d_device_context()->EndDraw();

	return S_OK;
}

void renderer_window::on_comment( const std::shared_ptr<comment_sample> comment )
{
	buffer_.push_back( comment );
	renderer_.add( *reinterpret_cast<const comment_base*>( ( buffer_.cend() - 1 )->get() ) );
}

void renderer_window::on_comment_mode_changed( const comment_mode mode )
{
	renderer_.set_mode( mode );
}