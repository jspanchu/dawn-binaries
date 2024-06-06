// SPDX-FileCopyrightText: Copyright (c) Jaswant Sai Panchumarti
// SPDX-License-Identifier: Apache-2.0
#include "weld.h"

#include "GLFW/glfw3.h"
#include "glfw3webgpu.h"

#include <cstdlib>
#include <iostream>
#include <vector>

/**
 * Utility function to get a WebGPU adapter, so that
 *     WGPUAdapter adapter = requestAdapterSync(options);
 * is roughly equivalent to
 *     const adapter = await navigator.gpu.requestAdapter(options);
 */
WGPUAdapter requestAdapterSync(WGPUInstance instance,
                               WGPURequestAdapterOptions const *options) {
  struct UserData {
    WGPUAdapter adapter = nullptr;
    bool requestEnded = false;
  };
  UserData userData;

  auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status,
                                  WGPUAdapter adapter, char const *message,
                                  void *pUserData) {
    UserData &userData = *reinterpret_cast<UserData *>(pUserData);
    if (status == WGPURequestAdapterStatus_Success) {
      userData.adapter = adapter;
    } else {
      std::cout << "Could not get WebGPU adapter: " << message << std::endl;
    }
    userData.requestEnded = true;
  };

  wgpuInstanceRequestAdapter(instance /* equivalent of navigator.gpu */,
                             options, onAdapterRequestEnded, (void *)&userData);

  return userData.adapter;
}
/**
 * Utility function to get a WebGPU device, so that
 *     WGPUAdapter device = requestDeviceSync(adapter, options);
 * is roughly equivalent to
 *     const device = await adapter.requestDevice(descriptor);
 * It is very similar to requestAdapter
 */
WGPUDevice requestDeviceSync(WGPUAdapter adapter,
                             WGPUDeviceDescriptor const *descriptor) {
  struct UserData {
    WGPUDevice device = nullptr;
    bool requestEnded = false;
  };
  UserData userData;

  auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status,
                                 WGPUDevice device, char const *message,
                                 void *pUserData) {
    UserData &userData = *reinterpret_cast<UserData *>(pUserData);
    if (status == WGPURequestDeviceStatus_Success) {
      userData.device = device;
    } else {
      std::cout << "Could not get WebGPU device: " << message << std::endl;
    }
    userData.requestEnded = true;
  };

  wgpuAdapterRequestDevice(adapter, descriptor, onDeviceRequestEnded,
                           (void *)&userData);

  return userData.device;
}
// We also add an inspect device function:
void inspectDevice(WGPUDevice device) {
  std::vector<WGPUFeatureName> features;
  size_t featureCount = wgpuDeviceEnumerateFeatures(device, nullptr);
  features.resize(featureCount);
  wgpuDeviceEnumerateFeatures(device, features.data());

  std::cout << "Device features:" << std::endl;
  for (auto f : features) {
    std::cout << " - " << f << std::endl;
  }

  WGPUSupportedLimits limits = {};
  limits.nextInChain = nullptr;
  bool success = wgpuDeviceGetLimits(device, &limits);
  if (success) {
    std::cout << "Device limits:" << std::endl;
    std::cout << " - maxTextureDimension1D: "
              << limits.limits.maxTextureDimension1D << std::endl;
    std::cout << " - maxTextureDimension2D: "
              << limits.limits.maxTextureDimension2D << std::endl;
    std::cout << " - maxTextureDimension3D: "
              << limits.limits.maxTextureDimension3D << std::endl;
    std::cout << " - maxTextureArrayLayers: "
              << limits.limits.maxTextureArrayLayers << std::endl;
    // Extra device limits
  }
}

std::ostream &operator<<(std::ostream &os, WGPUFeatureName &featureName) {
  switch (featureName) {
  case WGPUFeatureName_Undefined:
    os << "Undefined";
    break;
  case WGPUFeatureName_DepthClipControl:
    os << "DepthClipControl";
    break;
  case WGPUFeatureName_Depth32FloatStencil8:
    os << "Depth32FloatStencil8";
    break;
  case WGPUFeatureName_TimestampQuery:
    os << "TimestampQuery";
    break;
  case WGPUFeatureName_TextureCompressionBC:
    os << "TextureCompressionBC";
    break;
  case WGPUFeatureName_TextureCompressionETC2:
    os << "TextureCompressionETC2";
    break;
  case WGPUFeatureName_TextureCompressionASTC:
    os << "TextureCompressionASTC";
    break;
  case WGPUFeatureName_IndirectFirstInstance:
    os << "IndirectFirstInstance";
    break;
  case WGPUFeatureName_ShaderF16:
    os << "ShaderF16";
    break;
  case WGPUFeatureName_RG11B10UfloatRenderable:
    os << "RG11B10UfloatRenderable";
    break;
  case WGPUFeatureName_BGRA8UnormStorage:
    os << "BGRA8UnormStorage";
    break;
  case WGPUFeatureName_Float32Filterable:
    os << "Float32Filterable";
    break;
#if defined(WEBGPU_DAWN_CPP_BACKEND)
  case WGPUFeatureName_DawnInternalUsages:
    os << "DawnInternalUsages";
    break;
  case WGPUFeatureName_DawnMultiPlanarFormats:
    os << "DawnMultiPlanarFormats";
    break;
  case WGPUFeatureName_DawnNative:
    os << "DawnNative";
    break;
  case WGPUFeatureName_ChromiumExperimentalTimestampQueryInsidePasses:
    os << "ChromiumExperimentalTimestampQueryInsidePasses";
    break;
  case WGPUFeatureName_ImplicitDeviceSynchronization:
    os << "ImplicitDeviceSynchronization";
    break;
  case WGPUFeatureName_SurfaceCapabilities:
    os << "SurfaceCapabilities";
    break;
  case WGPUFeatureName_TransientAttachments:
    os << "TransientAttachments";
    break;
  case WGPUFeatureName_MSAARenderToSingleSampled:
    os << "MSAARenderToSingleSampled";
    break;
  case WGPUFeatureName_DualSourceBlending:
    os << "DualSourceBlending";
    break;
  case WGPUFeatureName_D3D11MultithreadProtected:
    os << "D3D11MultithreadProtected";
    break;
  case WGPUFeatureName_ANGLETextureSharing:
    os << "ANGLETextureSharing";
    break;
  case WGPUFeatureName_ChromiumExperimentalSubgroups:
    os << "ChromiumExperimentalSubgroups";
    break;
  case WGPUFeatureName_ChromiumExperimentalSubgroupUniformControlFlow:
    os << "ChromiumExperimentalSubgroupUniformControlFlow";
    break;
  case WGPUFeatureName_PixelLocalStorageCoherent:
    os << "PixelLocalStorageCoherent";
    break;
  case WGPUFeatureName_PixelLocalStorageNonCoherent:
    os << "PixelLocalStorageNonCoherent";
    break;
  case WGPUFeatureName_Unorm16TextureFormats:
    os << "Unorm16TextureFormats";
    break;
  case WGPUFeatureName_Snorm16TextureFormats:
    os << "Snorm16TextureFormats";
    break;
  case WGPUFeatureName_MultiPlanarFormatExtendedUsages:
    os << "MultiPlanarFormatExtendedUsages";
    break;
  case WGPUFeatureName_MultiPlanarFormatP010:
    os << "MultiPlanarFormatP010";
    break;
  case WGPUFeatureName_HostMappedPointer:
    os << "HostMappedPointer";
    break;
  case WGPUFeatureName_MultiPlanarRenderTargets:
    os << "MultiPlanarRenderTargets";
    break;
  case WGPUFeatureName_MultiPlanarFormatNv12a:
    os << "MultiPlanarFormatNv12a";
    break;
  case WGPUFeatureName_FramebufferFetch:
    os << "FramebufferFetch";
    break;
  case WGPUFeatureName_BufferMapExtendedUsages:
    os << "BufferMapExtendedUsages";
    break;
  case WGPUFeatureName_AdapterPropertiesMemoryHeaps:
    os << "AdapterPropertiesMemoryHeaps";
    break;
  case WGPUFeatureName_AdapterPropertiesD3D:
    os << "AdapterPropertiesD3D";
    break;
  case WGPUFeatureName_AdapterPropertiesVk:
    os << "AdapterPropertiesVk";
    break;
  case WGPUFeatureName_R8UnormStorage:
    os << "R8UnormStorage";
    break;
  case WGPUFeatureName_FormatCapabilities:
    os << "FormatCapabilities";
    break;
  case WGPUFeatureName_DrmFormatCapabilities:
    os << "DrmFormatCapabilities";
    break;
  case WGPUFeatureName_Norm16TextureFormats:
    os << "Norm16TextureFormats";
    break;
  case WGPUFeatureName_SharedTextureMemoryVkDedicatedAllocation:
    os << "SharedTextureMemoryVkDedicatedAllocation";
    break;
  case WGPUFeatureName_SharedTextureMemoryAHardwareBuffer:
    os << "SharedTextureMemoryAHardwareBuffer";
    break;
  case WGPUFeatureName_SharedTextureMemoryDmaBuf:
    os << "SharedTextureMemoryDmaBuf";
    break;
  case WGPUFeatureName_SharedTextureMemoryOpaqueFD:
    os << "SharedTextureMemoryOpaqueFD";
    break;
  case WGPUFeatureName_SharedTextureMemoryZirconHandle:
    os << "SharedTextureMemoryZirconHandle";
    break;
  case WGPUFeatureName_SharedTextureMemoryDXGISharedHandle:
    os << "SharedTextureMemoryDXGISharedHandle";
    break;
  case WGPUFeatureName_SharedTextureMemoryD3D11Texture2D:
    os << "SharedTextureMemoryD3D11Texture2D";
    break;
  case WGPUFeatureName_SharedTextureMemoryIOSurface:
    os << "SharedTextureMemoryIOSurface";
    break;
  case WGPUFeatureName_SharedTextureMemoryEGLImage:
    os << "SharedTextureMemoryEGLImage";
    break;
  case WGPUFeatureName_SharedFenceVkSemaphoreOpaqueFD:
    os << "SharedFenceVkSemaphoreOpaqueFD";
    break;
  case WGPUFeatureName_SharedFenceVkSemaphoreSyncFD:
    os << "SharedFenceVkSemaphoreSyncFD";
    break;
  case WGPUFeatureName_SharedFenceVkSemaphoreZirconHandle:
    os << "SharedFenceVkSemaphoreZirconHandle";
    break;
  case WGPUFeatureName_SharedFenceDXGISharedHandle:
    os << "SharedFenceDXGISharedHandle";
    break;
  case WGPUFeatureName_SharedFenceMTLSharedEvent:
    os << "SharedFenceMTLSharedEvent";
    break;
  case WGPUFeatureName_SharedBufferMemoryD3D12Resource:
    os << "SharedBufferMemoryD3D12Resource";
    break;
  case WGPUFeatureName_StaticSamplers:
    os << "StaticSamplers";
    break;
  case WGPUFeatureName_YCbCrVulkanSamplers:
    os << "YCbCrVulkanSamplers";
    break;
  case WGPUFeatureName_ShaderModuleCompilationOptions:
    os << "ShaderModuleCompilationOptions";
    break;
#endif
  case WGPUFeatureName_Force32:
    os << "Force32";
    break;
  default:
    os << "Unknown (0x" << std::hex << int(featureName) << std::dec << ")";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, WGPUBackendType &backendType) {
  switch (backendType) {
  case WGPUBackendType_Undefined:
    os << "Undefined";
    break;
  case WGPUBackendType_Null:
    os << "Null";
    break;
  case WGPUBackendType_WebGPU:
    os << "WebGPU";
    break;
  case WGPUBackendType_D3D11:
    os << "D3D11";
    break;
  case WGPUBackendType_D3D12:
    os << "D3D12";
    break;
  case WGPUBackendType_Metal:
    os << "Metal";
    break;
  case WGPUBackendType_Vulkan:
    os << "Vulkan";
    break;
  case WGPUBackendType_OpenGL:
    os << "OpenGL";
    break;
  case WGPUBackendType_OpenGLES:
    os << "OpenGLES";
    break;
  case WGPUBackendType_Force32:
    os << "Force32";
    break;
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, WGPUAdapterType &adapterType) {
  switch (adapterType) {
  case WGPUAdapterType_DiscreteGPU:
    os << "DiscreteGPU";
    break;
  case WGPUAdapterType_IntegratedGPU:
    os << "IntegratedGPU";
    break;
  case WGPUAdapterType_CPU:
    os << "CPU";
    break;
  case WGPUAdapterType_Unknown:
    os << "Unknown";
    break;
  case WGPUAdapterType_Force32:
    os << "Force32";
    break;
  }
  return os;
}

class Application {
public:
  // Initialize everything and return true if it went all right
  bool Initialize();

  // Uninitialize everything that was initialized
  void Terminate();

  // Draw a frame and handle events
  void MainLoop();

  // Return true as long as the main loop should keep on running
  bool IsRunning();

private:
  WGPUTextureView GetNextSurfaceTextureView();

private:
  // We put here all the variables that are shared between init and main loop
  GLFWwindow *window;
  WGPUDevice device;
  WGPUQueue queue;
  WGPUSurface surface;
};

bool Application::Initialize() {
  // Open window
  if (!glfwInit()) {
    return false;
  }
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(640, 480, "Learn WebGPU", nullptr, nullptr);

  if (!weldInitialize()) {
    std::cerr << "Failed to initialize weld!\n";
    return false;
  }
  WGPUInstance instance = wgpuCreateInstance(nullptr);

  std::cout << "Requesting adapter..." << std::endl;
  surface = glfwGetWGPUSurface(instance, window);
  WGPURequestAdapterOptions adapterOpts = {};
  adapterOpts.nextInChain = nullptr;
  adapterOpts.compatibleSurface = surface;
  adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;
  WGPUAdapter adapter = requestAdapterSync(instance, &adapterOpts);
  std::cout << "Got adapter: " << adapter << std::endl;

  wgpuInstanceRelease(instance);

  std::cout << "Requesting device..." << std::endl;
  WGPUDeviceDescriptor deviceDesc = {};
  deviceDesc.nextInChain = nullptr;
  deviceDesc.label = "My Device";
  deviceDesc.requiredFeatureCount = 0;
  deviceDesc.requiredLimits = nullptr;
  deviceDesc.defaultQueue.nextInChain = nullptr;
  deviceDesc.defaultQueue.label = "The default queue";
  deviceDesc.deviceLostCallback = [](WGPUDeviceLostReason reason,
                                     char const *message,
                                     void * /* pUserData */) {
    std::cout << "Device lost: reason " << reason;
    if (message)
      std::cout << " (" << message << ")";
    std::cout << std::endl;
  };
  device = requestDeviceSync(adapter, &deviceDesc);
  std::cout << "Got device: " << device << std::endl;

  wgpuAdapterRelease(adapter);

  auto onDeviceError = [](WGPUErrorType type, char const *message,
                          void * /* pUserData */) {
    std::cout << "Uncaptured device error: type " << type;
    if (message)
      std::cout << " (" << message << ")";
    std::cout << std::endl;
  };
  wgpuDeviceSetUncapturedErrorCallback(device, onDeviceError,
                                       nullptr /* pUserData */);

  queue = wgpuDeviceGetQueue(device);

  // Configure the surface
  WGPUSurfaceConfiguration config = {};
  config.nextInChain = nullptr;

  // Configuration of the textures created for the underlying swap chain
  config.width = 640;
  config.height = 480;
  config.usage = WGPUTextureUsage_RenderAttachment;
  WGPUTextureFormat surfaceFormat =
      wgpuSurfaceGetPreferredFormat(surface, adapter);
  config.format = surfaceFormat;

  // And we do not need any particular view format:
  config.viewFormatCount = 0;
  config.viewFormats = nullptr;
  config.device = device;
  config.presentMode = WGPUPresentMode_Fifo;
  config.alphaMode = WGPUCompositeAlphaMode_Auto;

  wgpuSurfaceConfigure(surface, &config);

  return true;
}

void Application::Terminate() {
  // Unconfigure the surface
  wgpuSurfaceUnconfigure(surface);
  wgpuQueueRelease(queue);
  wgpuSurfaceRelease(surface);
  wgpuDeviceRelease(device);
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Application::MainLoop() {
  glfwPollEvents();

  // Get the next target texture view
  WGPUTextureView targetView = GetNextSurfaceTextureView();
  if (!targetView)
    return;

  // Create a command encoder for the draw call
  WGPUCommandEncoderDescriptor encoderDesc = {};
  encoderDesc.nextInChain = nullptr;
  encoderDesc.label = "My command encoder";
  WGPUCommandEncoder encoder =
      wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

  // Create the render pass that clears the screen with our color
  WGPURenderPassDescriptor renderPassDesc = {};
  renderPassDesc.nextInChain = nullptr;

  // The attachment part of the render pass descriptor describes the target
  // texture of the pass
  WGPURenderPassColorAttachment renderPassColorAttachment = {};
  renderPassColorAttachment.view = targetView;
  renderPassColorAttachment.resolveTarget = nullptr;
  renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
  renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
  renderPassColorAttachment.clearValue = WGPUColor{0.9, 0.1, 0.2, 1.0};
#ifndef WEBGPU_BACKEND_WGPU
  renderPassColorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif // NOT WEBGPU_BACKEND_WGPU

  renderPassDesc.colorAttachmentCount = 1;
  renderPassDesc.colorAttachments = &renderPassColorAttachment;
  renderPassDesc.depthStencilAttachment = nullptr;
  renderPassDesc.timestampWrites = nullptr;

  // Create the render pass and end it immediately (we only clear the screen but
  // do not draw anything)
  WGPURenderPassEncoder renderPass =
      wgpuCommandEncoderBeginRenderPass(encoder, &renderPassDesc);
  wgpuRenderPassEncoderEnd(renderPass);
  wgpuRenderPassEncoderRelease(renderPass);

  // Finally encode and submit the render pass
  WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
  cmdBufferDescriptor.nextInChain = nullptr;
  cmdBufferDescriptor.label = "Command buffer";
  WGPUCommandBuffer command =
      wgpuCommandEncoderFinish(encoder, &cmdBufferDescriptor);
  wgpuCommandEncoderRelease(encoder);

  std::cout << "Submitting command..." << std::endl;
  wgpuQueueSubmit(queue, 1, &command);
  wgpuCommandBufferRelease(command);
  std::cout << "Command submitted." << std::endl;

  // At the enc of the frame
  wgpuTextureViewRelease(targetView);
  wgpuSurfacePresent(surface);

#if defined(WEBGPU_BACKEND_DAWN)
  wgpuDeviceTick(device);
#elif defined(WEBGPU_BACKEND_WGPU)
  wgpuDevicePoll(device, false, nullptr);
#endif
}

bool Application::IsRunning() { return !glfwWindowShouldClose(window); }

WGPUTextureView Application::GetNextSurfaceTextureView() {
  // Get the surface texture
  WGPUSurfaceTexture surfaceTexture;
  wgpuSurfaceGetCurrentTexture(surface, &surfaceTexture);
  if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success) {
    return nullptr;
  }

  // Create a view for this surface texture
  WGPUTextureViewDescriptor viewDescriptor;
  viewDescriptor.nextInChain = nullptr;
  viewDescriptor.label = "Surface texture view";
  viewDescriptor.format = wgpuTextureGetFormat(surfaceTexture.texture);
  viewDescriptor.dimension = WGPUTextureViewDimension_2D;
  viewDescriptor.baseMipLevel = 0;
  viewDescriptor.mipLevelCount = 1;
  viewDescriptor.baseArrayLayer = 0;
  viewDescriptor.arrayLayerCount = 1;
  viewDescriptor.aspect = WGPUTextureAspect_All;
  WGPUTextureView targetView =
      wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);

  return targetView;
}

int main() {
  Application app;

  if (!app.Initialize()) {
    return EXIT_FAILURE;
  }

  while (app.IsRunning()) {
    app.MainLoop();
  }

  return EXIT_SUCCESS;
}
