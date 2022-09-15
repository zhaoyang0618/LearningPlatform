export module ModReach;

//����ע���������û�е���
struct Data { // declare a type not exported
	int value;
};

export struct Customer { // declare an exported type
private:
	Data data;
public:
	Customer(int i)
		: data{ i } {
	}
	Data getData() const { // yield not exported type
		return data;
	}
};
