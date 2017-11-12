#pragma once

#include <wrl.h>
#include <d3d11_2.h>
#include "Game.h"
#include "RenderTarget.h"

namespace Rendering
{
	class FullScreenRenderTarget final : public Library::RenderTarget
	{
	public:
		FullScreenRenderTarget(Library::Game& game);
		FullScreenRenderTarget(const FullScreenRenderTarget& rhs) = delete;
		FullScreenRenderTarget& operator=(const FullScreenRenderTarget& rhs) = delete;

		ID3D11ShaderResourceView* OutputTexture() const;
		ID3D11RenderTargetView* RenderTargetView() const;
		ID3D11DepthStencilView* DepthStencilView() const;

		void Begin() override;
		void End() override;

	private:
		Library::Game* mGame;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mOutputTexture;
	};
}