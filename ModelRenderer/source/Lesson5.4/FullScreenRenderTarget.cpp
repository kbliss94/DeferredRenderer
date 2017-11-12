#include "pch.h"
#include "FullScreenRenderTarget.h"

using namespace Microsoft::WRL;
using namespace Library;

namespace Rendering
{
	FullScreenRenderTarget::FullScreenRenderTarget(Game& game):
		mGame(&game), mRenderTargetView(nullptr), mDepthStencilView(nullptr), mOutputTexture(nullptr)
	{
		SIZE renderTargetSize = game.RenderTargetSize();

		D3D11_TEXTURE2D_DESC fullScreenTextureDesc = { 0 };
		fullScreenTextureDesc.Width = renderTargetSize.cx;
		fullScreenTextureDesc.Height = renderTargetSize.cy;
		fullScreenTextureDesc.MipLevels = 1;
		fullScreenTextureDesc.ArraySize = 1;
		fullScreenTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		fullScreenTextureDesc.SampleDesc.Count = 1;
		fullScreenTextureDesc.SampleDesc.Quality = 0;
		fullScreenTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		fullScreenTextureDesc.Usage = D3D11_USAGE_DEFAULT;

		ComPtr<ID3D11Texture2D> fullScreenTexture;
		ThrowIfFailed(game.Direct3DDevice()->CreateTexture2D(&fullScreenTextureDesc, nullptr, fullScreenTexture.ReleaseAndGetAddressOf()), "ID3D11Device::CreateTexture2D() failed.");
		ThrowIfFailed(game.Direct3DDevice()->CreateShaderResourceView(fullScreenTexture.Get(), nullptr, mOutputTexture.ReleaseAndGetAddressOf()), "ID3D11Device::CreateShaderResourceView() failed.");
		ThrowIfFailed(game.Direct3DDevice()->CreateRenderTargetView(fullScreenTexture.Get(), nullptr, mRenderTargetView.ReleaseAndGetAddressOf()), "ID3D11Device::CreateRenderTargetView() failed.");

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.Width = renderTargetSize.cx;
		depthStencilDesc.Height = renderTargetSize.cy;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

		ComPtr<ID3D11Texture2D> depthStencilBuffer;
		ThrowIfFailed(game.Direct3DDevice()->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateTexture2D() failed.");
		ThrowIfFailed(game.Direct3DDevice()->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, mDepthStencilView.ReleaseAndGetAddressOf()), "ID3D11Device::CreateDepthStencilView() failed.");
	}

	ID3D11ShaderResourceView* FullScreenRenderTarget::OutputTexture() const
	{
		return mOutputTexture.Get();
	}

	ID3D11RenderTargetView* FullScreenRenderTarget::RenderTargetView() const
	{
		return mRenderTargetView.Get();
	}

	ID3D11DepthStencilView* FullScreenRenderTarget::DepthStencilView() const
	{
		return mDepthStencilView.Get();
	}

	void FullScreenRenderTarget::Begin()
	{
		RenderTarget::Begin(mGame->Direct3DDeviceContext(), 1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get(), mGame->Viewport());
	}

	void FullScreenRenderTarget::End()
	{
		RenderTarget::End(mGame->Direct3DDeviceContext());
	}
}