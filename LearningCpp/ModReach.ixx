export module ModReach;

//必须注意这个类型没有导出
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
