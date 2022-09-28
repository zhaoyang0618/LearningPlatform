using Caviar.SharedKernel.Entities.User;

namespace Architecture.Web.Wasm.Pages
{
    public partial class Login
    {
        public UserLogin ApplicationUser { get; set; } = new UserLogin()
        {
            UserName = "admin",
            Password = "123456",
            RememberMe = true,
        };

        string? style;

        protected override void OnInitialized()
        {
            string backgroundImage = "_content/Caviar.AntDesignUI/images/grov.jpg";
            style = $"min-height:100vh;background-image: url({backgroundImage});";
            base.OnInitialized();
        }
    }
}
