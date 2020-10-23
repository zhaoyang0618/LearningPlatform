using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Taiji.Utils
{
    public class TxtToolBLL
    {
        #region 构造函数
        public TxtToolBLL()
        {
            //
            // TODO: 在此处添加构造函数逻辑
            //
        }
        #endregion


        #region 文件操作函数
        /// <summary>
        /// 读取文本文件
        /// </summary>
        /// <param name="FilePath"></param>
        /// <returns></returns>
        public static string Reader(string FilePath)
        {
            StreamReader sr = null;
            string Lstr = string.Empty; ;
            if (File.Exists(FilePath))
            {
                //sr=File.OpenText(FilePath);
                sr = new StreamReader(FilePath, System.Text.Encoding.Default);
                //sr.StreamReader
                Lstr = sr.ReadToEnd();
                sr.Close();
            }


            return Lstr;

        }
        /// <summary>
        /// 写文本文件
        /// </summary>
        /// <param name="Content"></param>
        /// <param name="FilePath"></param>
        /// <returns></returns>
        public static bool Writer(string Content, string FilePath)
        {

            bool check = false;
            if (!File.Exists(FilePath)) return false;
            StreamWriter Sw;
            Sw = File.AppendText(FilePath);
            try
            {

                Sw.WriteLine(Content);
                check = true;

            }
            catch
            {
                check = false;
            }
            finally
            {
                Sw.Close();
                Sw = null;
            }

            return check;

        }
        /// <summary>
        /// 移动或重命名
        /// </summary>
        /// <param name="FilePath"></param>
        /// <param name="DestFilePath"></param>
        /// <returns></returns>
        public static bool FileMove(string FilePath, string DestFilePath)
        {
            bool check = false;

            try
            {
                if (File.Exists(FilePath))
                {
                    File.Move(FilePath, DestFilePath);

                }
                check = true;
            }
            catch
            {
                check = false;
            }

            return check;
        }
        public static bool FileCopy(string FilePath, string DestFilePath)
        {
            bool check = false;

            try
            {
                if (File.Exists(FilePath))
                {
                    File.Copy(FilePath, DestFilePath);

                }
                check = true;
            }
            catch
            {
                check = false;
            }

            return check;
        }
        public static bool Creater(string FilePath)
        {
            bool check = false;
            StreamWriter sc = null;
            try
            {
                if (File.Exists(FilePath))
                {
                    File.Delete(FilePath);

                }

                sc = File.CreateText(FilePath);
                check = true;
            }
            catch
            {
                check = false;
            }
            sc.Close();
            return check;



        }
        /// <summary>
        /// 创建任意各式的文件
        /// </summary>
        /// <param name="FilePath"></param>
        /// <returns></returns>
        public static bool CreaterFile(string FilePath)
        {
            bool check = false;
            System.IO.FileStream sc = null;
            try
            {
                if (File.Exists(FilePath))
                {
                    File.Delete(FilePath);
                }

                sc = File.Create(FilePath, 1024);
                check = true;
            }
            catch (System.Exception e)
            {
                string ss = e.Message;
                check = false;
            }
            sc.Close();
            return check;



        }
        /// <summary>
        /// 将字节数据写到文件的指定位置
        /// </summary>
        /// <param name="Content">数据</param>
        /// <param name="BeginWriteIndex">开始写入的位置</param>
        /// <param name="BeginIndex">数据开始读出的位置</param>
        /// <param name="Length">写入数据的长度</param>
        /// <param name="FilePath">文件路径和名称</param>
        /// <returns></returns>
        public static bool Writerbyte(byte[] Content, int BeginWriteIndex, int BeginIndex, int Length, string FilePath)
        {
            bool check = false;
            if (!File.Exists(FilePath)) return false;
            System.IO.FileStream Sw = File.OpenWrite(FilePath);
            Sw.Seek(BeginWriteIndex, System.IO.SeekOrigin.Begin);
            try
            {
                Sw.Write(Content, BeginIndex, Length);
                check = true;
            }
            catch
            {
                check = false;
            }
            Sw.Close();
            return check;
        }



        /// <summary>
        /// 从文件指定位置读取数据
        /// </summary>
        /// <param name="Content">数据内容</param>
        /// <param name="BeginReadIndex">开始读取的位置</param>
        /// <param name="BeginIndex">数据开始写入的位置</param>
        /// <param name="Length">要读取的数据长度</param>
        /// <param name="FilePath">文件路径和名称</param>
        /// <returns></returns>
        public static bool Readbyte(byte[] Content, int BeginReadIndex, int BeginIndex, int Length, string FilePath)
        {
            bool check = false;
            if (!File.Exists(FilePath)) return false;

            System.IO.FileStream Sw = File.OpenRead(FilePath);
            Sw.Seek(BeginReadIndex, System.IO.SeekOrigin.Begin);
            try
            {
                Sw.Read(Content, BeginIndex, Length);
                check = true;
            }
            catch
            {
                check = false;
            }
            Sw.Close();
            return check;
        }


        /// <summary>
        /// 从指定的路径文件读取全部的数据
        /// </summary>
        /// <param name="FilePath"></param>
        /// <returns></returns>
        public static byte[] ReaderBytes(string FilePath)
        {

            if (!File.Exists(FilePath)) return null;
            return File.ReadAllBytes(FilePath);
        }
        #endregion

        #region 字符串分割函数
        /// <summary>
        /// 字符串分割函数
        /// </summary>
        /// <param name="source">被分割的数据</param>
        /// <param name="key">分割符</param>
        /// <returns></returns>
        public static string[] StringSplit(string source, string key)
        {
            //string[]  strarry=new string[100];
            int point1 = 0, keylenth = key.Length;
            ArrayList list = new ArrayList();


            while (source != string.Empty)
            {
                point1 = source.IndexOf(key);
                if (point1 > 0)
                {
                    list.Add(source.Substring(0, point1).Trim());

                    source = source.Substring(point1 + keylenth);
                }
                else
                {
                    list.Add(source);
                    source = string.Empty;
                }
            }
            string[] strarry = new string[list.Count];
            for (int i = 0; i < list.Count; i++)
            {
                strarry[i] = list[i].ToString();
            }
            //System.Array.Copy(list,strarry,list.Count);
            return strarry;

        }

        /// <summary>
        /// 字符右截取
        /// </summary>
        /// <param name="source">被截取的数据</param>
        /// <param name="len">长度</param>
        /// <returns></returns>
        public static string StringRight(string source, int len)
        {
            return source.Substring(source.Length - len, len);
        }

        /// <summary>
        /// 字符左截取
        /// </summary>
        /// <param name="source">被截取的数据</param>
        /// <param name="len">长度</param>
        /// <returns></returns>
        public static string StringLeft(string source, int len)
        {
            return source.Substring(0, len);
        }
        #endregion

        #region XOR检验算法
        public static int XORCheck(string stringArray)
        {
            return XORCheck(System.Text.Encoding.Default.GetBytes(stringArray));
        }
        public static int XORCheck(byte[] byteArray)
        {

            int sscresult = 0;
            for (int i = 0; i < byteArray.Length; i++)
            {
                sscresult ^= (int)byteArray[i];
            }
            return sscresult;
        }
        public static int XORCheck(byte[] byteArray, int StartIndex, int CheckLength)
        {
            if (byteArray.Length < StartIndex + CheckLength) return 0;
            int sscresult = 0;
            for (int i = StartIndex; i < CheckLength + StartIndex; i++)
            {
                sscresult ^= (int)byteArray[i];
            }
            return sscresult;
        }
        public static int ADDCheck(byte[] byteArray, int StartIndex, int CheckLength)
        {
            if (byteArray.Length < StartIndex + CheckLength) return 0;
            int sscresult = 0;
            for (int i = StartIndex; i < CheckLength + StartIndex; i++)
            {
                sscresult += (int)byteArray[i];
            }
            return sscresult;
        }

        public static int ADDCheckFF(byte[] byteArray, int StartIndex, int CheckLength)
        {
            if (byteArray.Length < StartIndex + CheckLength) return 0;
            int sscresult = 0;
            for (int i = StartIndex; i < CheckLength + StartIndex; i++)
            {
                sscresult += (int)byteArray[i];
                sscresult = sscresult & 0xff;
            }
            return sscresult;
        }
        #endregion

        #region 字节数组的操作方法
        /// <summary>
        /// 字节数组进行比较
        /// </summary>
        /// <param name="source">字节数组1</param>
        /// <param name="StartIndex">开始比较索引</param>
        /// <param name="key">字节数组2</param>
        /// <param name="keyStartIndex">开始比较索引</param>
        /// <param name="Blength">总共比较的长度</param>
        /// <returns></returns>
        public static bool BinaryCompare(byte[] source, int StartIndex, byte[] key, int keyStartIndex, int Blength)
        {
            if (source.Length < Blength || key.Length < Blength) return false;
            if (source.Length - StartIndex < Blength || key.Length - keyStartIndex < Blength) return false;
            for (int i = 0; i < Blength; i++)
            {
                if (source[StartIndex + i] != key[keyStartIndex + i]) return false;
            }
            return true;
        }
        /// <summary>
        /// 字节数据查询
        /// </summary>
        /// <param name="source">数据源</param>
        /// <param name="key">要查找的数据</param>
        /// <returns>返回-1说明没有找到，其他值为第一次出现时，第一个字节所在位置序号</returns>
        public static int BinarySearch(byte[] source, byte[] key)
        {
            try
            {
                bool checker = false;
                int index = -1;
                for (int i = 0; i < source.Length - key.Length; i++)
                {
                    checker = true;
                    for (int k = 0; k < key.Length; k++)
                    {
                        if (source[i + k] != key[k])
                        {
                            checker = false;
                            break;
                        }
                    }
                    if (checker == true)
                    {
                        index = i;
                        break;
                    }

                }
                return index;
            }
            catch
            {
                return -1;
            }
        }
        public static int BinarySearch(byte[] source, byte[] key, int StartIndex)
        {
            return BinarySearch(source, key, StartIndex, source.Length);
        }
        public static int BinarySearch(byte[] source, byte[] key, int StartIndex, int Offset)
        {
            try
            {
                bool checker = false;
                int index = -1;
                for (int i = StartIndex; i < Offset - key.Length; i++)
                {
                    checker = true;
                    for (int k = 0; k < key.Length; k++)
                    {
                        if (source[i + k] != key[k])
                        {
                            checker = false;
                            break;
                        }
                    }
                    if (checker == true)
                    {
                        index = i;
                        break;
                    }

                }
                return index;
            }
            catch
            {
                return -1;
            }
        }
        public static string GetBytesHexStr(byte[] source, int StartIndex, int Offset)
        {
            if (source == null || source.Length == 0) return "";
            StringBuilder sb = new StringBuilder();
            for (int i = StartIndex; i < StartIndex + Offset && i < source.Length; i++)
            {
                sb.AppendFormat("{0:X2} ", source[i]);
            }
            return sb.ToString().TrimEnd();
        }
        public static string GetBytesHexStr(byte[] source)
        {
            if (source == null || source.Length == 0) return "";
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < source.Length; i++)
            {
                sb.AppendFormat("{0:X2} ", source[i]);

            }
            return sb.ToString();
        }
        public static string GetBytesHexdata(byte[] source)
        {
            if (source == null || source.Length == 0) return "";
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < source.Length; i++)
            {
                sb.AppendFormat("0x{0:X2},", source[i]);

            }
            return sb.ToString().TrimEnd(',');
        }


        /// <summary>
        /// 没有空格
        /// </summary>
        /// <param name="source"></param>
        /// <returns></returns>



        public static string GetBytesHexStrD(byte[] source, int StartIndex, int Offset)
        {
            if (source == null || source.Length == 0) return "";
            StringBuilder sb = new StringBuilder();
            for (int i = StartIndex; i < StartIndex + Offset && i < source.Length; i++)
            {
                sb.AppendFormat("{0:X2}", source[i]);
            }
            return sb.ToString().TrimEnd();
        }
        public static string GetBytesHexStrD(byte[] source)
        {
            if (source == null || source.Length == 0) return "";
            StringBuilder sb = new StringBuilder();

            for (int i = 0; i < source.Length; i++)
            {
                sb.AppendFormat("{0:X2}", source[i]);

            }
            return sb.ToString();
        }
        #endregion

        #region 整形变量与字节数组的相互转换
        public static byte[] int2Byte(long intValue, int bytesLength)
        {

            return int2Byte(intValue, bytesLength, true);
        }

        public static byte[] int2Byte(long intValue, int bytesLength, bool BigOrSmall)
        {
            byte[] b = new byte[bytesLength];
            if (BigOrSmall)
            {
                for (int i = 0; i < bytesLength; i++)
                {
                    b[i] = (byte)(intValue % 256);
                    intValue = intValue / 256;
                    //if (intValue == 0) break;
                }
            }
            else
            {
                for (int i = 0; i < bytesLength; i++)
                {
                    b[bytesLength - 1 - i] = (byte)(intValue % 256);
                    intValue = intValue / 256;
                    //if (intValue == 0) break;
                }
            }
            return b;
        }
        public static void int2Byte(long intValue, byte[] b, int StartIndex, int Offset)
        {

            int2Byte(intValue, b, StartIndex, Offset, true);


        }
        public static void int2Byte(long intValue, byte[] b, int StartIndex, int Offset, bool BigOrSmall)
        {
            if (BigOrSmall)
            {
                for (int i = 0; i < Offset; i++)
                {
                    b[i + StartIndex] = (byte)(intValue % 256);
                    intValue = intValue / 256;
                    //if (intValue == 0) break;
                }
            }
            else
            {
                for (int i = 0; i < Offset; i++)
                {
                    b[StartIndex + Offset - 1 - i] = (byte)(intValue % 256);
                    intValue = intValue / 256;
                    //if (intValue == 0) break;
                }
            }

        }
        public static int byte2Int(byte[] b)
        {

            return byte2Int(b, true);
        }
        public static int byte2Int(byte[] b, bool BigOrSmall)
        {
            int intValue = 0;
            if (BigOrSmall)
            {
                for (int i = 0; i < b.Length; i++)
                {
                    intValue += (b[i] & 0xFF) << (8 * i);

                }
            }
            else
            {
                for (int i = 0; i < b.Length; i++)
                {
                    intValue += (b[i] & 0xFF) << (8 * (b.Length - 1 - i));

                }
            }
            return intValue;
        }


        public static long byte2Int(byte[] b, int StartIndex, int Offset)
        {
            return byte2Int(b, StartIndex, Offset, true);
        }
        public static int byte2Int_I(byte[] b, int StartIndex, int Offset, bool BigOrSmall)
        {
            return Convert.ToInt32(byte2Int(b, StartIndex, Offset, BigOrSmall));
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="b"></param>
        /// <param name="StartIndex"></param>
        /// <param name="Offset"></param>
        /// <param name="BigOrSmall"> true- 高位在后 false -高位在前 </param>
        /// <returns></returns>
        public static long byte2Int(byte[] b, int StartIndex, int Offset, bool BigOrSmall)
        {
            if (!b.CheckAvailable()) return 0;
            if (b.Length < StartIndex + Offset) return 0;
            long intValue = 0;
            if (BigOrSmall)
            {
                for (int i = 0; i < Offset; i++)
                {

                    intValue += ((long)(b[i + StartIndex] & 0xFF)) << (8 * i);

                }
            }
            else
            {
                for (int i = 0; i < Offset; i++) //大头模式
                {
                    //intValue1=b[i+StartIndex] & 0xFF;
                    //intValue1=intValue1<<(8*(Offset-1-i));
                    intValue += ((long)(b[i + StartIndex] & 0xFF)) << (8 * (Offset - 1 - i));
                    //intValue+=intValue1;
                }
            }
            return intValue;

        }
        /// <summary>
        /// 转换成 Int32
        /// </summary>
        /// <param name="b"></param>
        /// <param name="StartIndex"></param>
        /// <param name="Offset"></param>
        /// <param name="BigOrSmall"></param>
        /// <returns></returns>
        public static int byte2RealInt(byte[] b, int StartIndex, int Offset, bool BigOrSmall)
        {
            return Convert.ToInt32(byte2Int(b, StartIndex, Offset, BigOrSmall));
        }
        public static long Hexbyte2Int(byte[] b, int StartIndex, int Offset)
        {
            long intValue = 0;
            StringBuilder strb = new StringBuilder(string.Empty);
            for (int i = 0; i < Offset; i++)
            {

                if ((int)b[i + StartIndex] < 16) strb.Append("0");
                strb.Append(((int)b[i + StartIndex]).ToString("X"));

            }
            try
            {
                intValue = long.Parse(strb.ToString());
            }
            catch { }
            return intValue;

        }


        public static void CopyStringTpbyte(ref byte[] data, int index, string Cont)
        {
            if (string.IsNullOrEmpty(Cont)) return;
            int count = Cont.Length / 2;
            for (int i = 0; i < count; i++)
            {
                data[i + index] = (byte)Convert.ToInt32("0x" + Cont.Substring(i * 2, 2), 16);
            }
        }
        /// <summary>
        /// 从16进制字符串转换成bytes
        /// </summary>
        /// <param name="datas"></param>
        /// <returns></returns>
        public static byte[] GetBytesByHexStringData(string datas)
        {
            if (string.IsNullOrEmpty(datas)) return null;
            string msgDataStr = datas.Replace(" ", "");
            byte[] msgDatabytes = new byte[msgDataStr.Length / 2];
            CopyStringTpbyte(ref msgDatabytes, 0, msgDataStr);
            return msgDatabytes;
        }
        #endregion

        #region 格式转换函数
        public static object ToInt(string s)
        {
            if (s == null || s == String.Empty)
                return System.DBNull.Value;
            else
                return Convert.ToInt32(s);
        }

        public static object ToInt(string s, string flag)
        {
            if (s == flag)
                return System.DBNull.Value;
            else
                return Convert.ToInt32(s);
        }

        public static object ToInt(string s, int DefaultValue)
        {
            if (s == null || s == String.Empty)
                return DefaultValue;
            else
                return Convert.ToInt32(s);
        }

        public static object ToString(string s)
        {
            if (s == null || s == String.Empty)
                return System.DBNull.Value;
            else
                return s;
        }
        public static object Tobool(string s)
        {
            if (s == null || s == String.Empty)
                return System.DBNull.Value;
            else
                return Convert.ToBoolean(s);
        }

        public static object ToString(string s, string flag)
        {
            if (s == flag)
                return System.DBNull.Value;
            else
                return s;
        }

        public static object ToDateTime(string s)
        {
            if (s == null || s == String.Empty)
                return System.DBNull.Value;
            else
                return Convert.ToDateTime(s);
        }


        public static object ToDateTime(string s, string flag)
        {
            if (s == flag)
                return System.DBNull.Value;
            else
                return Convert.ToDateTime(s);
        }

        public static object ToDecimal(string s)
        {
            if (s == null || s == String.Empty)
                return System.DBNull.Value;
            else
                return Convert.ToDecimal(s);
        }

        public static object ToDecimal(string s, string flag)
        {
            if (s == flag)
                return System.DBNull.Value;
            else
                return Convert.ToDecimal(s);
        }

        public static string IntToHEXStr(int v, int l)
        {
            string buf = string.Format("000000000000000000{0:x}", v);
            return buf.Substring(buf.Length - l, l);
        }
        public static string IntToDECStr(int v, int l)
        {
            string buf = string.Format("000000000000000000{0}", v);
            return buf.Substring(buf.Length - l, l);
        }


        public static string IntToDECStr(long v, int l)
        {
            string buf = string.Format("000000000000000000{0}", v);
            return buf.Substring(buf.Length - l, l);
        }
        #endregion       

        #region 常用数据转换
        /// <summary>
        /// 根据月份数据yyyyMM获得次月第一天
        /// </summary>
        /// <param name="MonthName">yyyyMM</param>
        /// <returns></returns>
        public static string GetFirstDay(string MonthName)
        {
            return string.Format("{0}-{1}-01", MonthName.Substring(0, 4), MonthName.Substring(4));
        }
        /// <summary>
        /// 根据月份数据yyyyMM获得次月最后一天
        /// </summary>
        /// <param name="MonthName">yyyyMM</param>
        /// <returns></returns>
        public static string GetLastDay(string MonthName)
        {
            return DateTime.Parse(GetFirstDay(MonthName)).AddMonths(1).AddDays(-1).ToString("yyyy-MM-dd");
        }
        /// <summary>
        /// 获得里程显示字符串
        /// </summary>
        /// <param name="MeterV"></param>
        /// <returns></returns>
        public static string GetMilageStringByMeter(int MeterV)
        {
            return (MeterV / 1000).ToString("0.##");
        }
        /// <summary>
        /// 将秒数转换成 h:m:s 格式
        /// </summary>
        /// <param name="SecondV"></param>
        /// <returns></returns>
        public static string GetPersisStringBySecond(int SecondV)
        {
            StringBuilder sb = new StringBuilder();
            //if (SecondV >= 3600) 
            sb.AppendFormat("{0}:", SecondV / 3600);
            //if (SecondV >= 60) 
            sb.AppendFormat("{0}:", SecondV % 3600 / 60);
            sb.AppendFormat("{0}", SecondV % 60);

            return sb.ToString();
        }
        /// <summary>
        /// 将秒数转换成 h:m 格式
        /// </summary>
        /// <param name="SecondV"></param>
        /// <returns></returns>
        public static string GetPersisStringBySecond_hm(int SecondV)
        {
            StringBuilder sb = new StringBuilder();
            //if (SecondV >= 3600) 
            sb.AppendFormat("{0}:", SecondV / 3600);
            //if (SecondV >= 60) 
            sb.AppendFormat("{0}", SecondV % 3600 / 60);


            return sb.ToString();
        }
        public static string GetPersisStringBySecond_hm(long SecondV)
        {
            StringBuilder sb = new StringBuilder();
            //if (SecondV >= 3600) 
            sb.AppendFormat("{0}:", SecondV / 3600);
            //if (SecondV >= 60) 
            sb.AppendFormat("{0}", SecondV % 3600 / 60);


            return sb.ToString();
        }
        #endregion

        #region 时间变量与字节数组的相互转换
        public static void DateTime2Byte(DateTime datetimeV, byte[] b, int StartIndex, int length)
        {
            if (b == null || b.Length <= StartIndex) return;
            if (length < 7)
            {

                b[StartIndex] = (byte)(datetimeV.Year % 100);
                b[StartIndex + 1] = (byte)datetimeV.Month;
                b[StartIndex + 2] = (byte)datetimeV.Day;
                if (StartIndex + 3 < b.Length && length > 3)
                    b[StartIndex + 3] = (byte)datetimeV.Hour;
                if (StartIndex + 4 < b.Length && length > 4)
                    b[StartIndex + 4] = (byte)datetimeV.Minute;
                if (StartIndex + 5 < b.Length && length > 5)
                    b[StartIndex + 5] = (byte)datetimeV.Second;
            }

            if (length == 7)
            {
                b[StartIndex + 0] = (byte)(datetimeV.Year / 100);
                b[StartIndex + 1] = (byte)(datetimeV.Year % 100);
                b[StartIndex + 2] = (byte)datetimeV.Month;
                b[StartIndex + 3] = (byte)datetimeV.Day;
                b[StartIndex + 4] = (byte)datetimeV.Hour;
                b[StartIndex + 5] = (byte)datetimeV.Minute;
                b[StartIndex + 6] = (byte)datetimeV.Second;
            }


        }
        public static string DateTimeFromByte(byte[] b, int StartIndex, int length)
        {
            if (b == null || b.Length <= StartIndex) return "";

            StringBuilder strb = new StringBuilder(string.Empty);
            if (length < 7)
            {
                strb.Append(DateTime.Now.Year.ToString().Substring(0, 2));
                strb.AppendFormat("{0:00}", b[StartIndex]);
                strb.AppendFormat("-{0:00}", b[StartIndex + 1]);
                strb.AppendFormat("-{0:00}", b[StartIndex + 2]);
                if (StartIndex + 3 < b.Length && length > 3)
                    strb.AppendFormat(" {0:00}", b[StartIndex + 3]);
                if (StartIndex + 4 < b.Length && length > 4)
                    strb.AppendFormat(":{0:00}", b[StartIndex + 4]);
                if (StartIndex + 5 < b.Length && length > 5)
                    strb.AppendFormat(":{0:00}", b[StartIndex + 5]);
            }

            if (length == 7)
            {
                strb.AppendFormat("{0:00}", b[StartIndex + 0]);
                strb.AppendFormat("{0:00}", b[StartIndex + 1]);
                strb.AppendFormat("-{0:00}", b[StartIndex + 2]);
                strb.AppendFormat("-{0:00}", b[StartIndex + 3]);
                if (StartIndex + 4 < b.Length)
                    strb.AppendFormat(" {0:00}", b[StartIndex + 4]);
                if (StartIndex + 5 < b.Length)
                    strb.AppendFormat(":{0:00}", b[StartIndex + 5]);
                if (StartIndex + 6 < b.Length)
                    strb.AppendFormat(":{0:00}", b[StartIndex + 6]);
            }

            return strb.ToString();
        }



        /// <summary>
        /// 用7个字节表达时间 年占用2个字节
        /// 日期占用4个字节 时间占用3个字节 16进制数据
        /// </summary>
        /// <param name="datetimeV"></param>
        /// <param name="b"></param>
        /// <param name="StartIndex"></param>
        /// <param name="length"></param>
        public static void DateTime2Byte7(DateTime datetimeV, byte[] b, int StartIndex, int length)
        {
            if (b == null || b.Length <= StartIndex) return;

            b[StartIndex] = (byte)(datetimeV.Year / 100);
            b[StartIndex + 1] = (byte)(datetimeV.Year % 100);
            b[StartIndex + 2] = (byte)datetimeV.Month;
            b[StartIndex + 3] = (byte)datetimeV.Day;
            if (StartIndex + 4 < b.Length && length > 4)
                b[StartIndex + 4] = (byte)datetimeV.Hour;
            if (StartIndex + 5 < b.Length && length > 5)
                b[StartIndex + 5] = (byte)datetimeV.Minute;
            if (StartIndex + 6 < b.Length && length > 6)
                b[StartIndex + 6] = (byte)datetimeV.Second;


        }

        /// <summary>
        /// 从7个字节转换为时间变量 
        /// 日期占用4个字节 时间占用3个字节 16进制数据
        /// </summary>
        /// <param name="b"></param>
        /// <param name="StartIndex"></param>
        /// <param name="length"></param>
        /// <returns></returns>
        public static string DateTimeFromByte7(byte[] b, int StartIndex, int length)
        {
            if (b == null || b.Length <= StartIndex) return "";

            StringBuilder strb = new StringBuilder(string.Empty);



            strb.AppendFormat("{0:00}", b[StartIndex + 0]);

            strb.AppendFormat("{0:00}", b[StartIndex + 1]);

            strb.AppendFormat("-{0:00}", b[StartIndex + 2]);

            strb.AppendFormat("-{0:00}", b[StartIndex + 3]);

            if (StartIndex + 4 < b.Length && length > 4)
                strb.AppendFormat(" {0:00}", b[StartIndex + 4]);

            if (StartIndex + 5 < b.Length && length > 5)
                strb.AppendFormat(":{0:00}", b[StartIndex + 5]);

            if (StartIndex + 6 < b.Length && length > 6)
                strb.AppendFormat(":{0:00}", b[StartIndex + 6]);

            return strb.ToString();
        }
        #endregion

        #region CRC16
        public static byte[] CRC16TableH_A001 = {
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00,0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,0x00, 0xC1, 0x81,
        0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,0x00, 0xC1, 0x81,0x40
        };
        public static byte[] CRC16TableL_A001 = {
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,0x05, 0xC5, 0xC4,
        0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB,0x0B, 0xC9, 0x09,
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE,0xDF, 0x1F, 0xDD,
        0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2,0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,0x36, 0xF6, 0xF7,
        0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E,0xFE, 0xFA, 0x3A,
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B,0x2A, 0xEA, 0xEE,
        0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27,0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,0x63, 0xA3, 0xA2,
        0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD,0x6D, 0xAF, 0x6F,
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8,0xB9, 0x79, 0xBB,
        0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4,0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,0x50, 0x90, 0x91,
        0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94,0x54, 0x9C, 0x5C,
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59,0x58, 0x98, 0x88,
        0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D,0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,0x41, 0x81, 0x80,0x40
        };
        /// <summary>
        /// CRC16 兼容效验 x16+x15+x2+1=8005(取反A001); 查表模式
        /// </summary>
        /// <param name="data"></param>
        /// <param name="startIndex"></param>
        /// <param name="length"></param>
        /// <returns></returns>
        public static int crc16All_A001(byte[] data, int startIndex, int length)
        {

            int uchCRCHi = 0xFF; /* CRC 的高字节初始化*/
            int uchCRCLo = 0xFF; /* CRC 的低字节初始化*/
            for (int i = startIndex; i < length + startIndex; i++)
            {
                int uIndex = uchCRCLo ^ data[i];
                uchCRCLo = uchCRCHi ^ CRC16TableH_A001[uIndex];
                uchCRCHi = CRC16TableL_A001[uIndex];
                uchCRCHi = uchCRCHi & 0xff;
                uchCRCLo = uchCRCLo & 0xff;
            }
            return uchCRCHi << 8 | uchCRCLo;
        }


        /// <summary>
        /// CRC16 兼容效验
        /// </summary>
        /// <param name="data">数据</param>
        /// <param name="start">起始地址</param>
        /// <param name="length">验证数据长度</param>
        /// <param name="poly">验证多项式:CRC-16:x16+x15+x2+1=8005(取反A001);CRC16-CCITT:x16+x12+x5+1-1021(取反8408);</param>
        /// <returns></returns>
        public static ushort crc16All(byte[] data, int startIndex, int length, ushort poly)
        {



            ushort CRCFull = 0xFFFF;
            //byte CRCHigh = 0xFF, CRCLow = 0xFF;
            char CRCLSB;

            for (int i = startIndex; i < length + startIndex; i++)
            {
                CRCFull = (ushort)(CRCFull ^ data[i]);

                for (int j = 0; j < 8; j++)
                {
                    CRCLSB = (char)(CRCFull & 0x0001);
                    CRCFull = (ushort)((CRCFull >> 1) & 0x7FFF);

                    if (CRCLSB == 1)
                    {
                        CRCFull = (ushort)(CRCFull ^ poly);
                        //CRCFull = (ushort)(CRCFull ^ 0x8408);
                    }

                }
            }
            //CRC[1] = CRCHigh = (byte)((CRCFull >> 8) & 0xFF);
            //CRC[0] = CRCLow = (byte)(CRCFull & 0xFF);


            return CRCFull;
        }

        /// <summary>
        /// 交通部下发的算法
        /// </summary>
        /// <param name="data"></param>
        /// <param name="polynomial">验证多项式:CRC-16:x16+x15+x2+1=8005(取反A001);CRC16-CCITT:x16+x12+x5+1-1021(取反8408);</param>
        /// <returns></returns>
        public static int getCRC_JiaoTongBu(byte[] data, int polynomial, int startIndex, int length)
        {
            int crc = 0xFFFF; // initial value
            // int polynomial = 0x1021; // 0001 0000 0010 0001 (0, 5, 12)
            for (int j = startIndex; j < length + startIndex; j++)
            {
                int b = data[j];
                for (int i = 0; i < 8; i++)
                {
                    bool bit = ((b >> (7 - i) & 1) == 1);
                    bool c15 = ((crc >> 15 & 1) == 1);
                    crc <<= 1;
                    if (c15 ^ bit)
                        crc ^= polynomial;
                }
            }
            crc &= 0xffff;
            return crc;
        }

        #endregion

    }
}
