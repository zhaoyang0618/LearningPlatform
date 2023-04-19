using Microsoft.EntityFrameworkCore;
using StockTraceApp.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace StockTraceApp.DB
{
    public class StockRepository : GenericRepository<Stock>
    {
        public StockRepository(DbContext context) : base(context)
        {
        }

        ///// <summary>
        ///// 批量添加
        ///// </summary>
        ///// <param name="list"></param>
        ///// <returns></returns>
        //public async Task<List<Stock>> AddCabinetList(List<Stock> list)
        //{
        //    System.Diagnostics.Debug.Assert(list != null && list.Count > 0);
        //    foreach (var v in list)
        //    {
        //        if (string.IsNullOrEmpty(v.Id))
        //        {
        //            v.Id = Guid.NewGuid().ToString();
        //        }
        //        dbSet.Add(v);
        //    }
        //    var ret = await context.SaveChangesAsync().ConfigureAwait(false);
        //    return list;
        //}

        //public async Task<Stock> AddCabinet(Stock node)
        //{
        //    System.Diagnostics.Debug.Assert(node != null);
        //    Insert(node);
        //    var ret = await context.SaveChangesAsync().ConfigureAwait(false);
        //    return node;
        //}

        //public async Task<int> UpdateCabinet(Stock node)
        //{
        //    if (node == null && string.IsNullOrEmpty(node.Id))
        //        return 0;

        //    Update(node);
        //    var ret = await context.SaveChangesAsync().ConfigureAwait(false);
        //    return ret;
        //}

        //public async Task<int> DeleteCabinet(Stock node)
        //{
        //    //首先删除该机柜相关的停留点
        //    var nodes = context.Set<Stock>();
        //    var nodeId = node.Id;
        //    var toDelete = nodes.Where(x => x.CabinetId != null && x.CabinetId.Equals(nodeId));
        //    if (toDelete != null)
        //    {
        //        context.RemoveRange(toDelete);
        //    }
        //    //
        //    Delete(node);
        //    var ret = await context.SaveChangesAsync().ConfigureAwait(false);
        //    return ret;
        //}

        //public async Task<int> DeleteCabinet(string id)
        //{
        //    ///直接删除
        //    ///需要删除关联的VisualObject

        //    var cabinet = new Stock() { Id = id };
        //    return await DeleteCabinet(cabinet);
        //}

        //public async Task<int> DeleteCabinets(string roomId)
        //{
        //    ///直接删除
        //    var list = dbSet.Where(x => x.RoomId.Equals(roomId));
        //    context.RemoveRange(list);
        //    var ret = await context.SaveChangesAsync().ConfigureAwait(false);
        //    return ret;
        //}

        //public async Task<Stock> GetById(string cabinetId)
        //{
        //    var query = dbSet.Where(x => x.Id.Equals(cabinetId)).Take(1);
        //    await query.Include(x => x.CabinetObjects).LoadAsync().ConfigureAwait(false);
        //    await query.Include(x => x.CabinetObjects).ThenInclude(x => x.Items).LoadAsync().ConfigureAwait(false);
        //    await query.Include(x => x.CabinetVirtualObjects).ThenInclude(x => x.CabinetObjectValueItem).LoadAsync().ConfigureAwait(false);
        //    var ret = await query.FirstOrDefaultAsync().ConfigureAwait(false);
        //    return ret;
        //}

        //public async Task<Stock> GetById2(string cabinetId)
        //{
        //    var query = dbSet.Where(x => x.Id.Equals(cabinetId))
        //        .Include(x => x.CabinetObjects)
        //        .Include(x => x.CabinetObjects).ThenInclude(x => x.Items)
        //        .Include(x => x.CabinetVirtualObjects).ThenInclude(x => x.CabinetObjectValueItem);
        //    var ret = await query.FirstOrDefaultAsync().ConfigureAwait(false);
        //    return ret;
        //}

        //public async Task<List<Stock>> ListByRoomId(string roomId)
        //{
        //    var query = dbSet.Where(x => x.RoomId.Equals(roomId))
        //        .Include(x => x.CabinetObjects)
        //        .Include(x => x.CabinetVirtualObjects).ThenInclude(x => x.CabinetObjectValueItem);
        //    var ret = await query.ToListAsync().ConfigureAwait(false);
        //    return ret;
        //}

        ///// <summary>
        ///// 更新部分字段
        ///// </summary>
        ///// <param name="cabinet"></param>
        ///// <returns></returns>
        //public async Task<int> UpdateCabinet(Stock cabinet, IEnumerable<string> columns)
        //{
        //    DbHelper<Stock>.DbColumnsUpdate(context, cabinet, columns);
        //    var ret = await context.SaveChangesAsync().ConfigureAwait(false);
        //    return ret;
        //}

        ///// <summary>
        ///// 更新部分字段
        ///// </summary>
        ///// <param name="cabinet"></param>
        ///// <returns></returns>
        //public async Task<int> UpdateRoom(Stock cabinet, params Expression<Func<Stock, object>>[] cols)
        //{
        //    DbHelper<Stock>.DbColumnsUpdate(context, cabinet, cols);
        //    var ret = await context.SaveChangesAsync().ConfigureAwait(false);
        //    return ret;
        //}

    }
}
