/// The file is a module unit because it starts with a declaration introducing that this is a file of module Mod1:

module Mod1; // implementation unit of module Mod1

double Customer::sumPrice() const
{
	double sum = 0.0;
	for (const Order& od : orders) {
		sum += od.count * od.price;
	}
	return sum;
}

double Customer::averagePrice() const
{
	if (orders.empty()) {
		return 0.0;
	}
	return sumPrice() / orders.size();
}