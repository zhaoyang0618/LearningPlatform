using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.ConstrainedExecution;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace SpiderClient.IoC
{
    internal class MyIOCContainer : IMyIOCContainer
    {
        private readonly Dictionary<string, Type> MyContianerDictionary = new Dictionary<string, Type>();

        /// <summary>
        /// 保存将抽象类型名称和实体类类型保存到字典
        /// </summary>
        /// <typeparam name="TFrom"></typeparam>
        /// <typeparam name="TTo"></typeparam>
        public void ResolveType<TFrom, TTo>()
        {
            MyContianerDictionary.Add(typeof(TFrom).FullName, typeof(TTo));
        }

        /// <summary>
        /// 创建对象
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        public T Resolve<T>()
        {
            Type type = MyContianerDictionary[typeof(T).FullName];
            return (T)CreateObject(type);
        }

        public object CreateObject(Type type)
        {
            //获取构造函数
            ConstructorInfo[] cons = type.GetConstructors();

            List<object> paraList = new List<object>();

            //如果构造函数数量大于0
            if (cons.Count() > 0)
            {
                //选择参数数量最多的构造函数
                ConstructorInfo con = cons.OrderByDescending(c => c.GetParameters().Length).FirstOrDefault();
                foreach (ParameterInfo para in con.GetParameters())
                {
                    Type paraType = para.ParameterType;
                    //字典容器查询出具体的参数对象类型
                    Type targetType = MyContianerDictionary[paraType.FullName];
                    //递归实例化所有参数对象，以及其依赖的对象，并添加到数组中
                    paraList.Add(this.CreateObject(targetType));
                }
            }

            //返回对象
            return Activator.CreateInstance(type, paraList.ToArray());
        }

        public static void Test()
        {
            IMyIOCContainer myIOC = new MyIOCContainer();
            myIOC.ResolveType<IPerson, Person>();
            myIOC.ResolveType<IHouse, House>();
            myIOC.ResolveType<ICar, Car>();
            myIOC.ResolveType<IMoney, Money>();
            IPerson person = myIOC.Resolve<IPerson>();
        }
    }

    //Money
    internal interface IMoney
    {
        bool IsSoMuch { get; set; }
    }
    internal class Money : IMoney
    {
        public bool IsSoMuch { get; set; }
        public Money()
        {
            Console.WriteLine("Money已被构造！！！");
        }
    }

    //Car 依赖 Money
    internal interface ICar
    {
        IMoney Money { get; set; }
        void Run();
    }

    internal class Car : ICar
    {
        public IMoney Money { get; set; }
        public Car(IMoney money)
        {
            this.Money = money;
            Console.WriteLine("Car已被构造！");
        }

        public void Run()
        {
            throw new NotImplementedException();
        }
    }

    //House 依赖 Money
    internal interface IHouse
    {
        IMoney Money { get; set; }
        void Live();
    }

    internal class House : IHouse
    {
        public IMoney Money { get; set; }

        public House(IMoney money)
        {
            this.Money = money;
            Console.WriteLine("House已被构造！！！");
        }

        public void Live() { }
    }

    //Person 依赖 House、Car
    internal interface IPerson
    {
        IHouse House { get; set; }
        ICar Car { get; set; }
        void Eat();
        void Sleep();
    }

    internal class Person : IPerson
    {
        public IHouse House { get; set; }
        public ICar Car { get; set; }

        public Person(IHouse house, ICar car)
        {
            House = house;
            Car = car;
            Console.WriteLine("Person已被构造！！！");
        }

        public void Eat() { }

        public void Sleep() { }
    }
}
