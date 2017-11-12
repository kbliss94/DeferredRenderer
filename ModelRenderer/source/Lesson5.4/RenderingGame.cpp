#include "pch.h"

using namespace std;
using namespace DirectX;
using namespace Library;

namespace Rendering
{
	const XMVECTORF32 RenderingGame::BackgroundColor = Colors::CornflowerBlue;
	const wstring RenderingGame::ColorFilterPixelShaders[] = { L"GrayscaleColorFilterPS.cso", L"GenericColorFilterPS.cso" };
	const string RenderingGame::ColorFilterDisplayNames[] = { "Grayscale", "Generic" };
	const float RenderingGame::BrightnessModulationRate = 1.0f;


	RenderingGame::RenderingGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback), mRenderStateHelper(*this),
		//off-screen render target
		mRenderTarget(nullptr), mFullScreenQuad(nullptr), mActiveColorFilter(ColorFilter::GrayScale), mPixelShadersByColorFilter(), 
		mGenericColorFilterPSConstantBuffer(nullptr), mGenericColorFilterPSConstantBufferData()
		//
	{

	}

	void RenderingGame::Initialize()
	{
		RasterizerStates::Initialize(mDirect3DDevice.Get());
		SamplerStates::Initialize(mDirect3DDevice.Get());

		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mMouse = make_shared<MouseComponent>(*this);
		mComponents.push_back(mMouse);
		mServices.AddService(MouseComponent::TypeIdClass(), mMouse.get());

		mGamePad = make_shared<GamePadComponent>(*this);
		mComponents.push_back(mGamePad);
		mServices.AddService(GamePadComponent::TypeIdClass(), mGamePad.get());

		mCamera = make_shared<FirstPersonCamera>(*this);
		mComponents.push_back(mCamera);
		mServices.AddService(Camera::TypeIdClass(), mCamera.get());

		mGrid = make_shared<Grid>(*this, mCamera);
		mComponents.push_back(mGrid);

		mPointLightDemo = make_shared<PointLightDemo>(*this, mCamera);
		mComponents.push_back(mPointLightDemo);

		Game::Initialize();

		mFpsComponent = make_shared<FpsComponent>(*this);
		mFpsComponent->Initialize();

		mCamera->SetPosition(0.0f, 2.5f, 25.0f);

		//off-screen render target
		mRenderTarget = make_unique<FullScreenRenderTarget>(*this);
		mFullScreenQuad = make_unique<FullScreenQuad>(*this);

		mFullScreenQuad->Initialize();
		mFullScreenQuad->SetCustomUpdateMaterial([&]
		{
			ID3D11DeviceContext* direct3DDeviceContext = Direct3DDeviceContext();

			direct3DDeviceContext->PSSetShader(mPixelShadersByColorFilter[mActiveColorFilter].Get(), nullptr, 0);

			static ID3D11ShaderResourceView* PSShaderResources[] = { mRenderTarget->OutputTexture() };
			direct3DDeviceContext->PSSetShaderResources(0, ARRAYSIZE(PSShaderResources), PSShaderResources);
			direct3DDeviceContext->PSSetSamplers(0, 1, SamplerStates::TrilinearWrap.GetAddressOf());

			if (mActiveColorFilter == ColorFilter::Generic)
			{
				direct3DDeviceContext->PSSetConstantBuffers(0, 1, mGenericColorFilterPSConstantBuffer.GetAddressOf());
			}
		});

		// Load compiled pixel shaders
		for (ColorFilter colorFilter = static_cast<ColorFilter>(0); colorFilter < ColorFilter::End; colorFilter = ColorFilter(static_cast<int>(colorFilter) + 1))
		{
			wstring shaderFilename = L"Content\\Shaders\\" + ColorFilterPixelShaders[static_cast<int>(colorFilter)];
			vector<char> compiledPixelShader;
			Utility::LoadBinaryFile(shaderFilename, compiledPixelShader);
			ThrowIfFailed(Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, mPixelShadersByColorFilter[colorFilter].ReleaseAndGetAddressOf()), "ID3D11Device::CreatedPixelShader() failed.");
		}

		// Create generic color filter constant buffer
		D3D11_BUFFER_DESC constantBufferDesc = { 0 };
		constantBufferDesc.ByteWidth = sizeof(GenericColorFilterPSConstantBuffer);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mGenericColorFilterPSConstantBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		// Initialize the generic color filter
		mGenericColorFilterPSConstantBufferData.ColorFilter = MatrixHelper::Identity;
		Direct3DDeviceContext()->UpdateSubresource(mGenericColorFilterPSConstantBuffer.Get(), 0, nullptr, &mGenericColorFilterPSConstantBufferData, 0, 0);
		//
	}

	void RenderingGame::Update(const GameTime &gameTime)
	{
		mFpsComponent->Update(gameTime);

		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape) || mGamePad->WasButtonPressedThisFrame(GamePadButtons::Back))
		{
			Exit();
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::G) || mGamePad->WasButtonPressedThisFrame(GamePadButtons::DPadUp))
		{
			mGrid->SetEnabled(!mGrid->Enabled());
			mGrid->SetVisible(!mGrid->Visible());
		}

		//toggling between color filters
		if (mKeyboard->WasKeyPressedThisFrame(Keys::K))
		{
			ColorFilter activeColorFilter = ColorFilter(static_cast<int>(mActiveColorFilter) + 1);
			if (activeColorFilter >= ColorFilter::End)
			{
				activeColorFilter = ColorFilter(0);
			}

			mActiveColorFilter = activeColorFilter;
		}

		if (mActiveColorFilter == ColorFilter::Generic)
		{
			UpdateGenericColorFilter(gameTime);
		}

		Game::Update(gameTime);
	}

	void RenderingGame::UpdateGenericColorFilter(const GameTime& gameTime)
	{
		static float brightness = 1.0f;

		if (mKeyboard->WasKeyPressedThisFrame(Keys::P) && brightness < 1.0f)
		{
			brightness += BrightnessModulationRate * gameTime.ElapsedGameTimeSeconds().count();
			brightness = XMMin<float>(brightness, 1.0f);
			XMStoreFloat4x4(&mGenericColorFilterPSConstantBufferData.ColorFilter, XMMatrixScaling(brightness, brightness, brightness));
			Direct3DDeviceContext()->UpdateSubresource(mGenericColorFilterPSConstantBuffer.Get(), 0, nullptr, &mGenericColorFilterPSConstantBufferData, 0, 0);
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::O) && brightness > 0.0f)
		{
			brightness -= BrightnessModulationRate * gameTime.ElapsedGameTimeSeconds().count();
			brightness = XMMax<float>(brightness, 0.0f);
			XMStoreFloat4x4(&mGenericColorFilterPSConstantBufferData.ColorFilter, XMMatrixScaling(brightness, brightness, brightness));
			Direct3DDeviceContext()->UpdateSubresource(mGenericColorFilterPSConstantBuffer.Get(), 0, nullptr, &mGenericColorFilterPSConstantBufferData, 0, 0);
		}
	}

	void RenderingGame::Draw(const GameTime &gameTime)
	{
		//rendering scene to off-screen render target
		mRenderTarget->Begin();

		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTarget->RenderTargetView(), reinterpret_cast<const float*>(&BackgroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mRenderTarget->DepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Game::Draw(gameTime);

		mRenderStateHelper.SaveAll();
		mFpsComponent->Draw(gameTime);
		mRenderStateHelper.RestoreAll();

		mRenderTarget->End();
		/////////////////

		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), reinterpret_cast<const float*>(&BackgroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Game::Draw(gameTime);

		mRenderStateHelper.SaveAll();
		mFpsComponent->Draw(gameTime);
		mRenderStateHelper.RestoreAll();

		HRESULT hr = mSwapChain->Present(1, 0);

		// If the device was removed either by a disconnection or a driver upgrade, we must recreate all device resources.
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			HandleDeviceLost();
		}
		else
		{
			ThrowIfFailed(hr, "IDXGISwapChain::Present() failed.");
		}
	}

	void RenderingGame::Shutdown()
	{
		SamplerStates::Shutdown();
		RasterizerStates::Shutdown();
	}

	void RenderingGame::Exit()
	{
		PostQuitMessage(0);
	}
}