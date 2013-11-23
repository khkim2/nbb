#include "test_cuda_kernel.h"

#include <numcpp.h>
#include <numcpp/device_array.h>

#include <iostream>
#include <cuda_runtime.h>

void test_cuda()
{
	using namespace std;

	const int N = 5;

	// Data on the host memory
	int a[N] = { 1, 2, 3, 4, 5 }, b[N] = { 3, 3, 3, 3, 3 }, c[N];

	// Print A
	for (int i = 0; i < N; i++)
		cout << a[i] << " ";
	cout << endl;

	// Print B
	for (int i = 0; i < N; i++)
		cout << b[i] << " ";
	cout << endl;

	// Data on the device memory
	int *a_d, *b_d, *c_d;

	// Allocate the device memory
	cudaMalloc((void **)&a_d, N * sizeof(int));
	cudaMalloc((void **)&b_d, N * sizeof(int));
	cudaMalloc((void **)&c_d, N * sizeof(int));

	// Copy from host to device
	cudaMemcpy(a_d, a, N * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(b_d, b, N * sizeof(int), cudaMemcpyHostToDevice);

	// Run kernel
	vecAdd(a_d, b_d, c_d, N);

	// Blocks until the device has completed all preceding requested tasks
	cudaThreadSynchronize();

	// Copy from device to host
	cudaMemcpy(c, c_d, N * sizeof(int), cudaMemcpyDeviceToHost);

	// Print C
	for (int i = 0; i < N; i++)
		cout << c[i] << " ";
	cout << endl;

	cudaFree(a_d);
	cudaFree(b_d);
	cudaFree(c_d);
}

void test_device_array()
{
	using namespace std;
	namespace np = numcpp;

	const int N = 5;

	// Data on the host memory
	auto a = np::array({1, 2, 3, 4, 5}), b = np::array({3, 3, 3, 3, 3});
	auto c = np::array<int>(5);

	// Print A
	for (int i = 0; i < N; i++)
		cout << a[i] << " ";
	cout << endl;

	// Print B
	for (int i = 0; i < N; i++)
		cout << b[i] << " ";
	cout << endl;

	// Data on the device memory
	auto a_d = np::device_array<int>(5), b_d = np::device_array<int>(5);
	auto c_d = np::device_array<int>(5);

	// Copy from host to device
	host_to_device(a_d, a);
	host_to_device(b_d, b);

	// Run kernel
	vecAdd(a_d, b_d, c_d, N);

	// Blocks until the device has completed all preceding requested tasks
	cudaThreadSynchronize();

	// Copy from device to host
	device_to_host(c, c_d);

	// Print C
	for (int i = 0; i < N; i++)
		cout << c[i] << " ";
	cout << endl;
}

int main(int argc, char **argv)
{
	test_cuda();
	test_device_array();

	return 0;
}
