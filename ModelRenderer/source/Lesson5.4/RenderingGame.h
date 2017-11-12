#pragma once

#include "Game.h"
#include "RenderStateHelper.h"
#include "FullScreenRenderTarget.h"
#include "FullScreenQuad.h"
#include <windows.h>
#include <functional>

namespace Library
{
	class KeyboardComponent;
	class MouseComponent;
	class GamePadComponent;
	class FpsComponent;
	class Camera;
	class Grid;

	//offscreen render target//
}

namespace Rendering
{
	class PointLightDemo;

	//offscreen render target//
	enum class ColorFilter
	{
		GrayScale = 0,
		Generic,
		End
	};
	////////////////////////////////////

	class RenderingGame final : public Library::Game
	{
	public:
		RenderingGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		virtual void Shutdown() override;

		void Exit();

		//offscreen render target
		void UpdateGenericColorFilter(const Library::GameTime& gameTime);
		//

	private:
		static const DirectX::XMVECTORF32 BackgroundColor;

		Library::RenderStateHelper mRenderStateHelper;
		std::shared_ptr<Library::KeyboardComponent> mKeyboard;
		std::shared_ptr<Library::MouseComponent> mMouse;
		std::shared_ptr<Library::GamePadComponent> mGamePad;
		std::shared_ptr<Library::FpsComponent> mFpsComponent;
		std::shared_ptr<Library::Camera> mCamera;
		std::shared_ptr<Library::Grid> mGrid;
		std::shared_ptr<PointLightDemo> mPointLightDemo;

		//offscreen render target//
		static const std::wstring ColorFilterPixelShaders[];
		static const std::string ColorFilterDisplayNames[];
		static const float BrightnessModulationRate;

		struct GenericColorFilterPSConstantBuffer
		{
			DirectX::XMFLOAT4X4 ColorFilter;

			GenericColorFilterPSConstantBuffer() : ColorFilter() { }

			GenericColorFilterPSConstantBuffer(const DirectX::XMFLOAT4X4& colorFilter) : ColorFilter(colorFilter) { }
		};

		std::unique_ptr<FullScreenRenderTarget> mRenderTarget;
		std::unique_ptr<FullScreenQuad> mFullScreenQuad;

		ColorFilter mActiveColorFilter;
		std::map<ColorFilter, Microsoft::WRL::ComPtr<ID3D11PixelShader>> mPixelShadersByColorFilter;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mGenericColorFilterPSConstantBuffer;
		GenericColorFilterPSConstantBuffer mGenericColorFilterPSConstantBufferData;
		////////////////////////////////////
	};
}
