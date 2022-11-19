#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <queue>

// Task 1
bool global_flag = false;
std::mutex mutex_for_flag;

std::queue<int> global_queue;

void DataPreparation()
{
	std::cout << "Enter some integer numbers: ";
	std::string str;
	std::getline(std::cin, str);
	std::istringstream sstr(str);
	int number;
	while (sstr >> number)
	{
		global_queue.push(number);
	}
	global_flag = true;
}

void DataProcessing()
{
	std::unique_lock<std::mutex> lock_for_global_flag(mutex_for_flag);
	while (!global_flag)
	{
		lock_for_global_flag.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		lock_for_global_flag.lock();
	}

	while (!global_queue.empty())
	{
		int number = global_queue.front();
		global_queue.pop();

		bool is_prime = true;
		for (int i = 2; i <= number / 2; i++)
		{
			if (number % i == 0)
			{
				is_prime = false;
				break;
			}
		}

		if (is_prime)
		{
			std::cout << number << " is a prime number\n";
		}
		else
		{
			std::cout << number << " is not a prime number\n";
		}
	}
}

// Task 2
int global_variable = 0;
std::condition_variable global_variable_cond;
std::mutex mutex_for_global_variable;
bool check_global_variable() { return global_variable == 1; }

void Awake()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "'Awake' function begins to inform the waiting threads\n";
	global_variable_cond.notify_all();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	global_variable = 1;
	std::cout << "'Awake' function again informs the waiting threads\n";
	global_variable_cond.notify_all();
}

void Waits()
{
	std::cout << "'Waits' function enters the standby state\n";
	std::unique_lock<std::mutex> lock_for_global_variable(mutex_for_global_variable);
	global_variable_cond.wait(lock_for_global_variable, check_global_variable);
	std::cout << "'Waits' function leaves the standby state\n";
}

// Task 3
void Notify()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	const int kThreadCount = 3;
	for (size_t i = 0; i < kThreadCount; i++)
	{
		global_variable = 1;
		global_variable_cond.notify_one();
	}
}

void Thread1()
{
	std::unique_lock<std::mutex> lock_for_global_variable(mutex_for_global_variable);
	global_variable_cond.wait(lock_for_global_variable, check_global_variable);
	std::cout << "Message from Thread1\n";
}

void Thread2()
{
	std::unique_lock<std::mutex> lock_for_global_variable(mutex_for_global_variable);
	global_variable_cond.wait(lock_for_global_variable, check_global_variable);
	std::cout << "Message from Thread2\n";
}

void Thread3()
{
	std::unique_lock<std::mutex> lock_for_global_variable(mutex_for_global_variable);
	global_variable_cond.wait(lock_for_global_variable, check_global_variable);
	std::cout << "Message from Thread3\n";
}

// Task 4
std::condition_variable global_queue_cond;
std::mutex mutex_for_queue;
std::queue<int> global_queue_v2;

void DataPreparationV2()
{
	std::cout << "Enter some integer numbers: ";
	std::string str;
	std::getline(std::cin, str);
	std::istringstream sstr(str);
	int number;
	while (sstr >> number)
	{
		global_queue_v2.push(number);
		global_queue_cond.notify_one();
	}
}

void DataProcessingV2()
{
	std::unique_lock<std::mutex> lock_for_global_queue(mutex_for_queue);
	global_queue_cond.wait(lock_for_global_queue, [] { return !global_queue_v2.empty(); });
	while (!global_queue_v2.empty())
	{
		int number = global_queue_v2.front();
		global_queue_v2.pop();

		bool is_prime = true;
		for (int i = 2; i <= number / 2; i++)
		{
			if (number % i == 0)
			{
				is_prime = false;
				break;
			}
		}

		if (is_prime)
		{
			std::cout << number << " is a prime number\n";
		}
		else
		{
			std::cout << number << " is not a prime number\n";
		}
	}
}

void Task1()
{
	std::thread data_preparation(DataPreparation);
	std::thread data_processing(DataProcessing);

	data_preparation.detach();
	data_processing.join();
}

void Task2()
{
	const int kThreadCount = 3;
	for (size_t i = 0; i < kThreadCount; i++)
	{
		std::thread wait_thread(Waits);
		wait_thread.detach();
	}

	std::thread awake_thread(Awake);
	awake_thread.join();
}

void Task3()
{
	std::thread thread1(Thread1);
	thread1.detach();
	std::thread thread2(Thread2);
	thread2.detach();
	std::thread thread3(Thread3);
	thread3.detach();

	std::thread notify(Notify);
	notify.join();
}

void Task4()
{
	std::thread data_preparation(DataPreparationV2);
	std::thread data_processing(DataProcessingV2);

	data_preparation.detach();
	data_processing.join();
}

int main()
{
	//Task1();
	//Task2();
	//Task3();
	Task4();
	system("pause");
	return 0;
}