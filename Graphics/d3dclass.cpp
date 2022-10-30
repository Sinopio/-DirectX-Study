#include "d3dclass.h"

D3DClass::D3DClass()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
	m_depthDisabledStencilState = 0;
	m_alphaEnableBlendingState = 0;
	m_alphaDisableBlendingState = 0;
	// ��ī�̵�
	m_rasterStateNoCulling = 0;
}

D3DClass::D3DClass(const D3DClass& other)
{
}

D3DClass::~D3DClass()
{
}

// DirectX11�� ��ü ������ �Ͼ�� ��
// screenWidth�� screenHeight�� SystemClass�� ���� �ʺ�� ���� -> D3X���� ���� ������ �ʱ�ȭ\
// screenDepth�� screenNear�� ���� �ý��ۿ��� �׷��� 3D���̰�
// vsync�� �������� ����� �ֻ����� ���� ������, ������ �Ұ����� ���ϴ°�(��������ȭ?)
bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth,
	float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;

	// vsync ����
	m_vsync_enabled = vsync;

	// �׷���ī��� ������ֻ��� ���� �޾ƿ´�.
	// DirectX �׷��� �������̽����丮�� �����.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// ���丮 ��ü�� ����Ͽ� ù��° �׷��� ī�� �������̽��� ���� adapter�� ����ϴ�.
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// ���(�����)�� ���� ù��° �ƴ��͸� �����մϴ�.
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// DXGI_FORMAT_R8G8B8A8_UNORM ����� ��� ���÷��� ���˿� �´� ����� ������ ���մϴ�.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// ������ ��� ����Ϳ� �׷���ī�� ������ ������ ����Ʈ�� �����մϴ�.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// ���÷��� ��忡 ���� ����Ʈ ������ ä�� �ִ´�.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// ���� ��� ���÷��� ��忡 ���� ȭ�� �ʺ�/���̿� �´� ���÷��� ��带 ã���ϴ�.
	// ������ ���� ã���� ������� ���ΰ�ħ ������ �и�� ���� ���� �����մϴ�.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// �����(�׷���ī��)�� description�� �����´�
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// ���� �׷���ī���� �޸� �뷮�� �ް�����Ʈ ������ ����
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// �׷���ī���� �̸��� char�� ���ڿ� �迭�� �ٲ� �� ����
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// ���÷��� ��� ����Ʈ�� �Ҵ� ����
	delete[] displayModeList;
	displayModeList = 0;
	// ��� ����� �Ҵ� ����
	adapterOutput->Release();
	adapterOutput = 0;
	// ����� �Ҵ�����
	adapter->Release();
	adapter = 0;
	// ���丮 �Ҵ�����
	factory->Release();
	factory = 0;

	// ����ü�� description �ʱ�ȭ
	// ������ ������ �Ѱ��� ��ϵǴ� ����Ʈ ���ۿ� �����
	// ������-> ����ۿ� �׸��� ����Ʈ���ۿ� swap�� => ����ü��
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// �ϳ��� ����� ���
	swapChainDesc.BufferCount = 1;

	// ������� �ʺ�� ���� ����
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// ����۷� �Ϲ����� 32bit ǥ�� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//�ֻ���(1�ʿ� ����ۿ� ����Ʈ���۰� �ٲ�� ����)
	//vsync������ true�� �����Ǿ� �ִٸ� �ֻ��� ����
	//vsync������ false�� �����Ǿ� �ִٸ� ������ ���� ���� �׸���.
	// �������� ���ΰ�ħ ������ ����
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	// ������� �뵵�� ����
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �������� �̷���� �ڵ��� ����
	swapChainDesc.OutputWindow = hwnd;

	// ��Ƽ���ø��� ����.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// â���, ��üȭ�� ����
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// ĵ������ ���İ� ��ĵ���̴��� �������� ��������(unspecified) ����
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ��µ� ������ ������� ������ ����
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// �߰� �ɼ� �÷��׸� ������� ����
	swapChainDesc.Flags = 0;

	//��� ������ DirectX11�� ����� ������?
	// DirectX 11 ���? ���ķ���?
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// ���� ü��, Direct3D ����̽�, Direct3D ����̽� ���ؽ�Ʈ�� ����
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
		&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL,
		&m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	//����� ���� �׷���ī�尡 �ٸ��Ƿ� ����� ��� �׷���ī�带 Ȯ���ϰ� �ֻ��� �������� �̲������

	// ������� �����и� �޾ƿ´�.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// ������� �����ͷ� RenderTargetView�� ����
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// ������� �����͸� ���̻� ������� �������� ��ȯ
	backBufferPtr->Release();
	backBufferPtr = 0;

	// ���� ���� �ʱ�ȭ
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// ���̹����� ����(description)�� �ۼ�
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// description�� ����Ͽ� �ؽ��ĸ� ����(2D�� �׷�����)
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// ���Ľ� ������ description�� �ʱ�ȭ
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// ���Ľ� ������ description�� �ۼ�
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// ���� �ȼ��� ��� ���ٽ� �۵�
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// �ĸ� �ȼ��� ��� ���ٽ� �۵�
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ���� ���Ľ� ���¸� ����
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// ���� ���ٽ� ���¸� ����
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// ����-���Ľ��� description�ʱ�ȭ
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// ���� ���Ľ��� description �ۼ�
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ���� ���Ľ��� �� ����
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc,
		&m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// ����Ÿ�ٺ�� ���� ���Ľ� �並 ��� ������ ���������ο� ���ε� ��Ų��
	// �� ������������ ���� �������� �� �� ������� �� ���ۿ� ����� �׷�����.
	// ����Ÿ�ٺ�� ���� ���Ľ� �並 ���� ��� ������ ���������ο� ���ε� ��Ų��
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// � ������ ��� �׸� �� �����ϴ� ������ȭ�� description
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	// �� �κп��� ���̾� ���� �ָ��� ���� ���� D3D11_FILL_WIREFRAME
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// �ۼ��� description���� ������ȭ�� ���¸� ����
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	//��ī�̵�
	// Setup a raster description which turns off back face culling.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the no culling rasterizer state.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateNoCulling);
	if (FAILED(result))
	{
		return false;
	}

	// ������ ȭ�� ����
	m_deviceContext->RSSetState(m_rasterState);

	// �������� ���� ����Ʈ ����
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// ����Ʈ ����
	m_deviceContext->RSSetViewports(1, &viewport);

	// ���� ����� ����
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// 3D�������� ���� ������� ����
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	// ���� ����̶�� ����� �����Ͽ� ������Ʈ�� ��ǥ�� ���� ��ǥ�� ��ȯ�ϴµ� ���
	// ��������� ���� ��ķ� �ʱ�ȭ
	D3DXMatrixIdentity(&m_worldMatrix);

	// �� ����� ��������� �츮�� �����ġ���� �����ִ°��� ����ϴµ� �̿�

	// �� ����� 3D��ü�� �ƴ϶� UI�� ���� 2D��Ҹ� �׸��µ� ���
	// 2D�������� ���� ���� ���� ����� ����
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the swap chain description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

// Initialize���� ����ߴ� �������� ����, ����
// Ǯ��ũ�� ��忡�� ����� ����ü�ο��� �� ���� ������ �߻�������
// ���������� ���� ������ â��� �� ��ȯ
void D3DClass::Shutdown()
{
	// ������ ������ ���� ��ȯ�Ͽ� ����ü�� �Ҵ���� ȸ��
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}
	if (m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}
	if (m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}
	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}
	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}
	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}
	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}
	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}
	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}
	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}
	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}
	//��ī�̵�
	if (m_rasterStateNoCulling)
	{
		m_rasterStateNoCulling->Release();
		m_rasterStateNoCulling = 0;
	}
	return;
}

// BeginScene�� �� �������� ���۸��� 3D ȭ���� �׸��� ������ �� ȣ��ȴ�. 
// �� �Լ��� ���۸� �� ������ �ʱ�ȭ�ϰ� �������� �̷�������� �غ��Ѵ�
void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// ������ ������ ����
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// ������� ������ �����.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// ���̹��۸� �����.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	return;
}

// �� �������� �������� ���� ü�ο��� ����ۿ� �׸� 3Dȭ���� ǥ���ϵ��� �ϰ� �Ѵ�
void D3DClass::EndScene()
{
	// �������� �Ϸ�Ǿ����Ƿ� ������� ���� ȭ�鿡 ǥ�� (vsync ���� ����)
	if (m_vsync_enabled)
	{
		// ���ΰ��� ������ ����
		m_swapChain->Present(1, 0);
	}
	else
	{
		// ������ ������ ǥ��
		m_swapChain->Present(0, 0);
	}
	return;
}

// Direct3D ����̽��� ����̽� ���ؽ�Ʈ�� ������ ��ȯ
ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}
ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

//���� ��ȯ
void D3DClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}
//���� ��ȯ
void D3DClass::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}
//���� ������� ��ȯ
void D3DClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

//�׷��� ī���� �̸��� ��밡���� �޸��� ���� ��ȯ
void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

void D3DClass::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	return;
}


void D3DClass::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}

void D3DClass::TurnOnAlphaBlending()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}


void D3DClass::TurnOffAlphaBlending()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

//��ī�̵�
void D3DClass::TurnOnCulling()
{
	// Set the culling rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	return;
}


void D3DClass::TurnOffCulling()
{
	// Set the no back face culling rasterizer state.
	m_deviceContext->RSSetState(m_rasterStateNoCulling);

	return;
}