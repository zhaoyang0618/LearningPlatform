using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Taiji.Utils
{
    #region StringFieldNameSortingSupport    
    public static class StringFieldNameSortingSupport
    {

        #region Private expression tree helpers
        private static LambdaExpression GenerateSelector<TEntity>(String propertyName, out Type resultType) where TEntity : class
        {
            // Create a parameter to pass into the Lambda expression (Entity => Entity.OrderByField).            
            var parameter = Expression.Parameter(typeof(TEntity), "Entity");
            //  create the selector part, but support child properties            
            PropertyInfo property;
            Expression propertyAccess;
            if (propertyName.Contains('.'))
            {
                // support to be sorted on child fields.                
                String[] childProperties = propertyName.Split('.');
                property = typeof(TEntity).GetProperty(childProperties[0]);
                propertyAccess = Expression.MakeMemberAccess(parameter, property);
                for (int i = 1; i < childProperties.Length; i++)
                {
                    property = property.PropertyType.GetProperty(childProperties[i]);
                    propertyAccess = Expression.MakeMemberAccess(propertyAccess, property);
                }
            }
            else
            {
                property = typeof(TEntity).GetProperty(propertyName);
                propertyAccess = Expression.MakeMemberAccess(parameter, property);
            }
            resultType = property.PropertyType;
            // Create the order by expression.           
            return Expression.Lambda(propertyAccess, parameter);
        }
        private static MethodCallExpression GenerateMethodCall<TEntity>(IQueryable<TEntity> source, string methodName, String fieldName) where TEntity : class
        {
            Type type = typeof(TEntity);
            Type selectorResultType;
            LambdaExpression selector = GenerateSelector<TEntity>(fieldName, out selectorResultType);
            MethodCallExpression resultExp = Expression.Call(typeof(Queryable), methodName,
                new Type[] { type, selectorResultType },
                source.Expression, Expression.Quote(selector));
            return resultExp;
        }
        #endregion
        public static IOrderedQueryable<TEntity> OrderBy<TEntity>(this IQueryable<TEntity> source, string fieldName) where TEntity : class
        {
            MethodCallExpression resultExp = GenerateMethodCall<TEntity>(source, "OrderBy", fieldName);
            return source.Provider.CreateQuery<TEntity>(resultExp) as IOrderedQueryable<TEntity>;
        }
        public static IOrderedQueryable<TEntity> OrderByDescending<TEntity>(this IQueryable<TEntity> source, string fieldName) where TEntity : class
        {
            MethodCallExpression resultExp = GenerateMethodCall<TEntity>(source, "OrderByDescending", fieldName);
            return source.Provider.CreateQuery<TEntity>(resultExp) as IOrderedQueryable<TEntity>;
        }
        public static IOrderedQueryable<TEntity> ThenBy<TEntity>(this IOrderedQueryable<TEntity> source, string fieldName) where TEntity : class
        {
            MethodCallExpression resultExp = GenerateMethodCall<TEntity>(source, "ThenBy", fieldName);
            return source.Provider.CreateQuery<TEntity>(resultExp) as IOrderedQueryable<TEntity>;
        }
        public static IOrderedQueryable<TEntity> ThenByDescending<TEntity>(this IOrderedQueryable<TEntity> source, string fieldName) where TEntity : class
        {
            MethodCallExpression resultExp = GenerateMethodCall<TEntity>(source, "ThenByDescending", fieldName);
            return source.Provider.CreateQuery<TEntity>(resultExp) as IOrderedQueryable<TEntity>;
        }
        public static IOrderedQueryable<TEntity> OrderUsingSortExpression<TEntity>(this IQueryable<TEntity> source, string sortExpression) where TEntity : class
        {
            String[] orderFields = sortExpression.Split(',');
            IOrderedQueryable<TEntity> result = null;
            for (int currentFieldIndex = 0; currentFieldIndex < orderFields.Length; currentFieldIndex++)
            {
                String[] expressionPart = orderFields[currentFieldIndex].Trim().Split(' ');
                String sortField = expressionPart[0];
                Boolean sortDescending = (expressionPart.Length == 2) && (expressionPart[1].Equals("DESC", StringComparison.OrdinalIgnoreCase));
                if (sortDescending)
                {
                    result = currentFieldIndex == 0 ? source.OrderByDescending(sortField) : result.ThenByDescending(sortField);
                }
                else
                {
                    result = currentFieldIndex == 0 ? source.OrderBy(sortField) : result.ThenBy(sortField);
                }
            }
            return result;
        }
    }
    #endregion
    public static class CommonExtFunction
    {
        #region 格式转换说明
        //字符  说明      示例 输出 
        //C     货币               string.Format("{0:C3}", 2) ＄2.000 
        //D     十进制             string.Format("{0:D3}", 2) 002 
        //E     科学计数法         1.20E+001 1.20E+001 
        //G     常规               string.Format("{0:G}", 2) 2 
        //N     用分号隔开的数字   string.Format("{0:N}", 250000) 250,000.00 
        //X     十六进制           string.Format("{0:X000}", 12) C 
        //    string.Format("{0:000.000}", 12.2) 012.200 

        //        1、字符串的数字格式

        //  string str1 =string.Format("{0:N1}",56789);               //result: 56,789.0
        //  string str2 =string.Format("{0:N2}",56789);               //result: 56,789.00
        //  string str3 =string.Format("{0:N3}",56789);               //result: 56,789.000
        //  string str8 =string.Format("{0:F1}",56789);               //result: 56789.0
        //  string str9 =string.Format("{0:F2}",56789);               //result: 56789.00
        //  string str11 =(56789 / 100.0).ToString("#.##");           //result: 567.89
        //  string str12 =(56789 / 100).ToString("#.##");             //result: 567
        //2、格式化货币（跟系统的环境有关，中文系统默认格式化人民币，英文系统格式化美元）

        //string.Format("{0:C}",0.2)


        // 结果为：￥0.20 （英文操作系统结果：$0.20）

        //默认格式化小数点后面保留两位小数，如果需要保留一位或者更多，可以指定位数

        //string.Format("{0:C1}",23.15)


        //结果为：￥23.2 （截取会自动四舍五入）
        //格式化多个Object实例 

        //string.Format("市场价：{0:C}，优惠价{1:C}",23.15,19.82)
        //3、格式化十进制的数字（格式化成固定的位数，位数不能少于未格式化前，只支持整形）

        //string.Format("{0:D3}",23) //结果为：023
        //string.Format("{0:D2}",1223) //结果为：1223，（精度说明符指示结果字符串中所需的最少数字个数。）


        // 4、用分号隔开的数字，并指定小数点后的位数

        //string.Format("{0:N}", 14200) //结果为：14,200.00 （默认为小数点后面两位）
        //string.Format("{0:N3}", 14200.2458) //结果为：14,200.246 （自动四舍五入）
        //5、格式化百分比

        //string.Format("{0:P}", 0.24583) //结果为：24.58% （默认保留百分的两位小数）
        //string.Format("{0:P1}", 0.24583) //结果为：24.6% （自动四舍五入）
        //6、零占位符和数字占位符

        //string.Format("{0:0000.00}", 12394.039) //结果为：12394.04
        //string.Format("{0:0000.00}", 194.039) //结果为：0194.04
        //string.Format("{0:###.##}", 12394.039) //结果为：12394.04
        //string.Format("{0:####.#}", 194.039) //结果为：194


        //下面的这段说明比较难理解，多测试一下实际的应用就可以明白了。
        //零占位符： 如果格式化的值在格式字符串中出现“0”的位置有一个数字，则此数字被复制到结果字符串中。小数点前最左边的“0”的位置和小数点后最右边的“0”的位置确定总在结果字符串中出现的数字范围。 “00”说明符使得值被舍入到小数点前最近的数字，其中零位总被舍去。
        //数字占位符： 如果格式化的值在格式字符串中出现“#”的位置有一个数字，则此数字被复制到结果字符串中。否则，结果字符串中的此位置不存储任何值。
        //请注意，如果“0”不是有效数字，此说明符永不显示“0”字符，即使“0”是字符串中唯一的数字。如果“0”是所显示的数字中的有效数字，则显示“0”字符。 “##”格式字符串使得值被舍入到小数点前最近的数字，其中零总被舍去。

        //7、日期格式化

        //string.Format("{0:d}",System.DateTime.Now) //结果为：2009-3-20 （月份位置不是03）
        //string.Format("{0:D}",System.DateTime.Now) //结果为：2009年3月20日
        //string.Format("{0:f}",System.DateTime.Now) //结果为：2009年3月20日 15:37
        // string.Format("{0:F}",System.DateTime.Now) //结果为：2009年3月20日 15:37:52
        // string.Format("{0:g}",System.DateTime.Now) //结果为：2009-3-20 15:38
        // string.Format("{0:G}",System.DateTime.Now) //结果为：2009-3-20 15:39:27
        // string.Format("{0:m}",System.DateTime.Now) //结果为：3月20日
        //string.Format("{0:t}",System.DateTime.Now) //结果为：15:41
        // string.Format("{0:T}",System.DateTime.Now) //结果为：15:41:50

        #endregion



        #region  byte[] 相关
        public static bool CheckAvailable<TSource>(this IEnumerable<TSource> Tlist)
        {

            return Tlist != null && Tlist.Count() > 0;
        }


        /// <summary>
        /// 将数据转换成Long 再转换成UTC DateTime
        /// </summary>
        /// <param name="Bs"></param>
        /// <param name="index"></param>
        /// <param name="length"></param>
        /// <param name="bigOrsmall"></param>
        /// <returns></returns>
        //public static DateTime Cof_ToLongUTC(this byte[] Bs, int index, int length, bool bigOrsmall)
        //{
        //    return TxtToolBLL.GetUTCtimeValueByLong(TxtToolBLL.byte2Int(Bs, index, length, bigOrsmall));
        //}
        /// <summary>
        /// 将大头模式INT转换成长整形
        /// </summary>
        /// <param name="Bs"></param>
        /// <param name="index"></param>
        /// <param name="length"></param>
        /// <returns></returns>
        public static long ToLong(this byte[] Bs, int index, int length)
        {
            return TxtToolBLL.byte2Int(Bs, index, length, false);
        }
        /// <summary>
        /// 将小头模式INT转换成长整形
        /// </summary>
        /// <param name="Bs"></param>
        /// <param name="index"></param>
        /// <param name="length"></param>
        /// <returns></returns>
        public static long ToLongX(this byte[] Bs, int index, int length)
        {
            return TxtToolBLL.byte2Int(Bs, index, length, true);
        }
        public static uint Cof_ToUInt(this byte[] Bs, int index, int length)
        {
            long l = TxtToolBLL.byte2Int(Bs, index, length, false);
            if (l > uint.MaxValue) return 0;
            return Convert.ToUInt32(l);
        }

        public static int Cof_ToInt(this byte[] Bs, int index, int length, bool bigOrsmall)
        {
            long ii = TxtToolBLL.byte2Int(Bs, index, length, bigOrsmall);
            if (ii >= int.MaxValue) return 0;
            return Convert.ToInt32(ii);
        }
        public static int Cof_ToInt(this byte[] Bs, int index, int length)
        {
            long l = TxtToolBLL.byte2Int(Bs, index, length, false);
            if (l > int.MaxValue)
            {
                return Convert.ToInt16((l & 0x7fffffff) - int.MaxValue - 1);
            }
            return Convert.ToInt32(l);
        }
        public static short Cof_ToShort(this byte[] Bs, int index, int length)
        {
            long l = TxtToolBLL.byte2Int(Bs, index, length, false);
            if (l > short.MaxValue)
            {
                return Convert.ToInt16((l & 0x7fff) - short.MaxValue - 1);
            }
            return Convert.ToInt16(l);
        }

        public static ushort Cof_ToUshort(this byte[] Bs, int index, int length)
        {
            long l = TxtToolBLL.byte2Int(Bs, index, length, false);
            if (l > ushort.MaxValue) return 0;
            return Convert.ToUInt16(l);
        }
        public static uint Cof_ToUint(this byte[] Bs, int index, int length)
        {
            long l = TxtToolBLL.byte2Int(Bs, index, length, false);
            if (l > uint.MaxValue) return 0;
            return Convert.ToUInt32(l);
        }
        /// <summary>
        /// 小头模式
        /// </summary>
        /// <param name="Bs"></param>
        /// <param name="index"></param>
        /// <param name="length"></param>
        /// <returns></returns>
        public static int Cof_ToIntX(this byte[] Bs, int index, int length)
        {
            return Convert.ToInt32(TxtToolBLL.byte2Int(Bs, index, length, true));
        }

        /// <summary>
        /// 添加Int值单byte
        /// </summary>
        /// <param name="Tlist"></param>
        /// <param name="ByteValue"></param>
        public static void Cof_AddByteByInt(this List<byte[]> Tlist, int ByteValue)
        {
            Tlist.Add(new byte[] { Convert.ToByte(ByteValue) });
        }

        ///// 转换成16进制字符串数据 不带空格
        ///// </summary>
        ///// <param name="Bs"></param>
        ///// <returns></returns>
        //public static string Cof_ToBCDString(this byte[] Bs, int index, int length)
        //{
        //    return TxtToolBLL.BCDbytesToString(Bs, index, length);
        //}
        ///// <summary>
        ///// 小端模式 低字节在前 高字节灾后
        ///// </summary>
        ///// <param name="Bs"></param>
        ///// <param name="index"></param>
        ///// <param name="length"></param>
        ///// <returns></returns>
        //public static string Cof_ToBCDStringX(this byte[] Bs, int index, int length)
        //{
        //    return TxtToolBLL.BCDbytesToStringX(Bs, index, length);
        //}
        //public static string Cof_ToDECString(this byte[] Bs, int index, int length)
        //{
        //    return TxtToolBLL.DECbytesToString(Bs, index, length);
        //}
        /// <summary>
        /// BCD字节变为整数 
        /// </summary>
        /// <param name="B"></param>
        /// <returns></returns>
        public static int ToBCDValue(this byte B)
        {
            return B.ToString("X2").Cof_ToSafeInt();
        }
        /// <summary>
        /// 转换成16进制字符串数据 带空格
        /// </summary>
        /// <param name="Bs"></param>
        /// <returns></returns>
        public static string ToHexString(this byte[] Bs)
        {
            return TxtToolBLL.GetBytesHexStr(Bs);
        }
        public static string ToHexStringS(this byte[] Bs, int Index, int offset)
        {
            return TxtToolBLL.GetBytesHexStr(Bs, Index, offset);
        }
        /// <summary>
        /// 转换成16进制字符串数据 不带空格
        /// </summary>
        /// <param name="Bs"></param>
        /// <returns></returns>
        public static string ToHexStringData(this byte[] Bs)
        {
            return TxtToolBLL.GetBytesHexStrD(Bs);
        }
        /// <summary>
        /// 转换成16进制字符串数据
        /// </summary>
        /// <param name="Bs"></param>
        /// <param name="Index"></param>
        /// <param name="offset"></param>
        /// <returns></returns>
        public static string ToHexStringData(this byte[] Bs, int Index, int offset)
        {
            return TxtToolBLL.GetBytesHexStrD(Bs, Index, offset);
        }
        /// <summary>
        /// 去掉前面的0
        /// </summary>
        /// <param name="Bs"></param>
        /// <param name="Index"></param>
        /// <param name="offset"></param>
        /// <returns></returns>
        public static string ToHexStringDataUNF0(this byte[] Bs, int Index, int offset)
        {
            string s = TxtToolBLL.GetBytesHexStrD(Bs, Index, offset).TrimStart('0');
            if (string.IsNullOrEmpty(s)) return "0";
            return s;
        }
        #endregion

        #region 格式转换


        /// <summary>
        /// 小时:分钟
        /// </summary>
        /// <param name="SecondV"></param>
        /// <returns></returns>
        public static string Cof_ToHHmm(this long SecondV)
        {
            return string.Format("{0}:{1}",
                SecondV / 3600,
                SecondV % 3600 / 60
                );
        }
        /// <summary>
        /// 转换为HH:mm:ss
        /// </summary>
        /// <param name="SecondV"></param>
        /// <returns></returns>
        public static string Cof_ToHHmmss(this long SecondV)
        {

            return string.Format("{0:00}:{1:00}:{2:00}",
                SecondV / 3600,
                SecondV % 3600 / 60,
                SecondV % 60
                );

        }
        /// <summary>
        /// 转换为h:m:s
        /// </summary>
        /// <param name="SecondV"></param>
        /// <returns></returns>
        public static string Cof_Tohms(this long SecondV)
        {
            return string.Format("{0}:{1}:{2}",
               SecondV / 3600,
               SecondV % 3600 / 60,
               SecondV % 60
               );

        }
        /// <summary>
        /// 泛型安全转换为字符串
        /// 如果为Not HasValue 则返回null
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="Tv"></param>
        /// <returns></returns>
        public static string Cof_SafeToString<T>(this T Tv)
        {
            if (Tv == null) return null;
            return Tv.ToString();
        }
        public static string Cof_EmptyOrStr(this int Iv, int EmptyV)
        {
            if (Iv == EmptyV) return "";
            return Iv.ToString();
        }
        public static string Cof_EmptyOrStr(this object Iv, int EmptyV)
        {
            if (Iv == null || (int)Iv == EmptyV) return "";
            return Iv.ToString();
        }
        public static string GetTimeString_hms(this int SecondV)
        {
            return string.Format("{0}:{1}:{2}", SecondV / 3600, SecondV % 3600 / 60, SecondV % 60);

        }
        public static string GetTimeString_hms2(this int SecondV)
        {
            return string.Format("{0:00}:{1:00}:{2:00}", SecondV / 3600, SecondV % 3600 / 60, SecondV % 60);

        }
        /// <summary>
        /// 将时间部分转换为Int 含义是距离当天凌晨的秒数
        /// </summary>
        /// <param name="Dt"></param>
        /// <returns></returns>
        public static int Cof_GetTimeInt_hms(this DateTime Dt)
        {
            return Dt.Hour * 3600 + Dt.Minute * 60 + Dt.Second;
        }



        public static string GetTimeString_hm(this int SecondV)
        {
            return string.Format("{0}:{1}", SecondV / 3600, SecondV % 3600 / 60);


        }


        public static double Cof_ToZerodouble(this int? o)
        {
            if (o.HasValue) return (double)o.Value;
            else return 0;
        }
        /// <summary>
        /// 整形泛型Tostring null 返回 "0"
        /// </summary>
        /// <param name="o"></param>
        /// <returns></returns>
        public static string Cof_ToZeroString(this int? o)
        {
            if (o.HasValue) return o.Value.ToString();
            else return "0";
        }
        public static int Cof_ToZeroValueInt(this int? o)
        {
            if (o.HasValue) return o.Value;
            else return 0;
        }
        public static T Cof_Value<T>(this T? Tge, T defautV) where T : struct
        {
            if (Tge.HasValue) return Tge.Value;
            return defautV;
        }
        public static string Cof_ToEmotyString(this int? o)
        {
            if (o.HasValue) return o.Value.ToString();
            else return "";
        }
        public static string Cof_ToEmptyString(this int? o)
        {
            if (o.HasValue) return o.Value.ToString();
            else return "";
        }

        public static bool Cof_ToSafeBool(this bool? o)
        {
            if (o.HasValue) return o.Value;
            else return false;
        }
        /// <summary>
        /// 转换成安全的整形
        /// </summary>
        /// <param name="S"></param>
        /// <returns></returns>
        public static int Cof_ToSafeInt(this string S)
        {
            if (string.IsNullOrEmpty(S)) return 0;
            int Sint = 0;
            if (int.TryParse(S, out Sint)) return Sint;
            return 0;

        }
        /// <summary>
        /// 转换成16进制数
        /// </summary>
        /// <param name="S"></param>
        /// <returns></returns>
        public static int Cof_ToSafeIntHex(this string S)
        {
            if (string.IsNullOrEmpty(S)) return 0;

            try
            {
                return Convert.ToInt32(S, 16);
            }
            catch
            {
            }
            return 0;
        }
        public static int Cof_ToSafeInt(this string S, int DefaultV)
        {
            if (string.IsNullOrEmpty(S)) return DefaultV;
            int Sint = 0;
            if (int.TryParse(S, out Sint)) return Sint;
            return DefaultV;

        }
        /// <summary>
        /// 默认返回-1
        /// </summary>
        /// <param name="S"></param>
        /// <returns></returns>
        public static int Cof_ToSafeInt_1(this string S)
        {
            if (string.IsNullOrEmpty(S)) return -1;
            int Sint = 0;
            if (int.TryParse(S, out Sint)) return Sint;
            return -1;

        }
        public static long Cof_ToSafeLong(this string S)
        {
            if (string.IsNullOrEmpty(S)) return 0;
            long Sint = 0;
            if (long.TryParse(S, out Sint)) return Sint;
            return 0;

        }

        public static int? Cof_ToSafeIntF(this string S)
        {
            if (string.IsNullOrEmpty(S)) return null;
            int Sint = 0;
            if (int.TryParse(S, out Sint)) return Sint;
            return null;

        }
        public static long? Cof_ToSafeLongF(this string S)
        {
            if (string.IsNullOrEmpty(S)) return null;
            long Sint = 0;
            if (long.TryParse(S, out Sint)) return Sint;
            return null;

        }
        public static double Cof_ToSafeDouble(this string S)
        {

            if (string.IsNullOrEmpty(S)) return 0;
            S = S.Trim();
            double Sint = 0;
            if (double.TryParse(S, out Sint)) return Sint;
            return 0;

        }
        public static decimal Cof_ToSafeDecimal(this string S)
        {
            if (string.IsNullOrEmpty(S)) return 0;
            decimal Sint = 0;
            if (decimal.TryParse(S, out Sint)) return Sint;
            return 0;

        }

        public static double? Cof_ToSafeDoubleF(this string S)
        {
            if (string.IsNullOrEmpty(S)) return null;
            double Sint = 0;
            if (double.TryParse(S, out Sint)) return Sint;
            return null;

        }
        public static decimal? Cof_ToSafeDecimalF(this string S)
        {
            if (string.IsNullOrEmpty(S)) return null;
            decimal Sint = 0;
            if (decimal.TryParse(S, out Sint)) return Sint;
            return null;

        }
        /// <summary>
        /// 将字符串转换成整形后 放到一个byte中
        /// </summary>
        /// <param name="S"></param>
        /// <returns></returns>
        public static byte Cof_ToSafeByte(this string S)
        {
            return Convert.ToByte(S.Cof_ToSafeInt());

        }
        /// <summary>
        /// 转换成安全的时间数据
        /// </summary>
        /// <param name="S"></param>
        /// <returns></returns>
        public static DateTime? Cof_ToSafeDateTime(this string S)
        {
            if (string.IsNullOrEmpty(S)) return null;
            DateTime SDateTime = DateTime.Now;
            if (DateTime.TryParse(S, out SDateTime)) return SDateTime;
            return null;
        }
        /// <summary>
        /// 如果出错则返回 DateTime.MinValue
        /// </summary>
        /// <param name="S"></param>
        /// <returns></returns>
        public static DateTime Cof_ToSafeDateTime2(this string S)
        {
            if (string.IsNullOrEmpty(S)) return DateTime.MinValue;
            DateTime SDateTime = DateTime.Now;
            if (DateTime.TryParse(S, out SDateTime)) return SDateTime;
            return DateTime.MinValue;
        }
        /// <summary>
        /// 转换成字节数组 GB2132
        /// </summary>
        /// <param name="S"></param>
        /// <returns></returns>
        public static byte[] Cof_ToBytes(this string S)
        {
            if (string.IsNullOrEmpty(S)) return null;
            return System.Text.Encoding.Default.GetBytes(S);
        }
        public static string Cof_ToFormatString(this decimal? o, string FormatS)
        {
            if (o.HasValue) return o.Value.ToString(FormatS);
            else return "0";
        }

        public static byte[] Cof_To4Bytes(this int D)
        {
            byte[] Dbytes = new byte[4];
            TxtToolBLL.int2Byte(D, Dbytes, 0, 4, false);
            return Dbytes;
        }
        public static byte[] Cof_To2Bytes(this int D)
        {
            byte[] Dbytes = new byte[2];
            TxtToolBLL.int2Byte(D, Dbytes, 0, 2, false);
            return Dbytes;
        }
        public static byte[] Cof_To2BytesX(this int D)
        {
            byte[] Dbytes = new byte[2];
            TxtToolBLL.int2Byte(D, Dbytes, 0, 2, true);
            return Dbytes;
        }
        public static byte[] Cof_To1Bytes(this int D)
        {
            return new byte[] { Convert.ToByte(D) };
        }
        public static byte Cof_ToByte(this int D)
        {
            return Convert.ToByte(D);
        }
        public static byte[] Cof_ToBytes(this long D, int Len)
        {
            byte[] Dbytes = new byte[Len];
            TxtToolBLL.int2Byte(D, Dbytes, 0, Len, false);
            return Dbytes;
        }
        public static byte[] Cof_ToBytesX(this long D, int Len)
        {
            byte[] Dbytes = new byte[Len];
            TxtToolBLL.int2Byte(D, Dbytes, 0, Len, true);
            return Dbytes;
        }
        #endregion


        #region 对于回车换行\r\n进行特殊处理

        /// <summary>
        /// Hex字符串转换成byte数组
        /// </summary>
        /// <param name="S"></param>
        /// <returns></returns>
        public static byte[] ToHexBytes(this string S)
        {
            return TxtToolBLL.GetBytesByHexStringData(S);
        }
        /// <summary>
        /// Hex字符串转换成byte数组后转为字符串 过滤不可见字符
        /// </summary>
        /// <param name="S"></param>
        /// <returns></returns>
        //public static string Cof_ToHexString(this string S)
        //{
        //    return TxtToolBLL.GetBytesByHexStringData(S).Cof_ToStringGB2312withFilter();
        //}
        #endregion

    }
}
