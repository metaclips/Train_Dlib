// Train_Dlib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <iostream>

using namespace std;
using namespace dlib;

int main(int argc,char** argv)
{
	try
	{
		if (argc != 2)
			cout << "Please drag path to MINST folder " << endl;
		std::vector<matrix<unsigned char>> training_images, testing_images;
		std::vector<unsigned long> training_labels, testing_labels;
		using net_type = loss_multiclass_log<
			fc<10,
			relu<fc<84,
			relu<fc<120,
			max_pool<2, 2, 2, 2, relu<con<16, 5, 5, 1, 1,
			max_pool<2, 2, 2, 2, relu<con<16, 5, 5, 1, 1,
			input<matrix<unsigned char>>
			>>>>>>>>>>>>;
		net_type net;
		dnn_trainer<net_type>trainer(net);
		trainer.be_verbose();
		trainer.set_learning_rate(0.01);
		trainer.set_min_learning_rate(0.00001);
		trainer.set_mini_batch_size(128);
		trainer.set_synchronization_file("mnist_sync", std::chrono::seconds(20));
		trainer.train(training_images, training_labels);
		net.clean();
		serialize("mnist_network.dat") << net;
		std::vector<unsigned long> predicted_labels = net(training_images);
		int num_right = 0;
		int num_wrong = 0;
		// And then let's see if it classified them correctly.
		for (size_t i = 0; i < training_images.size(); ++i)
		{
			if (predicted_labels[i] == training_labels[i])
				++num_right;
			else
				++num_wrong;

		}
		cout << "training num_right: " << num_right << endl;
		cout << "training num_wrong: " << num_wrong << endl;
		cout << "training accuracy:  " << num_right / (double)(num_right + num_wrong) << endl;

		// Let's also see if the network can correctly classify the testing images.  Since
		// MNIST is an easy dataset, we should see at least 99% accuracy.
		predicted_labels = net(testing_images);
		num_right = 0;
		num_wrong = 0;
		for (size_t i = 0; i < testing_images.size(); ++i)
		{
			if (predicted_labels[i] == testing_labels[i])
				++num_right;
			else
				++num_wrong;

		}
		cout << "testing num_right: " << num_right << endl;
		cout << "testing num_wrong: " << num_wrong << endl;
		cout << "testing accuracy:  " << num_right / (double)(num_right + num_wrong) << endl;


		// Finally, you can also save network parameters to XML files if you want to do
		// something with the network in another tool.  For example, you could use dlib's
		// tools/convert_dlib_nets_to_caffe to convert the network to a caffe model.
		net_to_xml(net, "lenet.xml");
		system("pause");
	}
	catch (std::exception& e)
	{
		cout << e.what();
	}
}