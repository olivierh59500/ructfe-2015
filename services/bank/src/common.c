#include "common.h"

#include <string.h>
#include <stdio.h>

int login_good(char* login) {
    int i;
    for (i = 0; login[i]; i += 1) {
        if (login[i] == ' ' || login[i] == '_' || login[i] == '=')
            continue;

        if (login[i] >= '0' && login[i] <= '9') {
            continue;
        }

        if (login[i] >= 'a' && login[i] <= 'z') {
            continue;
        }

        if (login[i] >= 'A' && login[i] <= 'Z') {
            continue;
        } 
        return 0;
    }

    return 1;
}

int account_good(char* account) {
    return login_good(account);
}

void print_header(int show_logout) {
    printf("Content-type: text/html;charset=UTF-8\r\n\r\n");

    printf("<!DOCTYPE html>\n");
    printf("<html lang='en-US'>\n");
    printf("<head>\n");
    printf("    <meta charset='utf-8'>\n");
    printf("    <meta http-equiv='X-UA-Compatible' content='IE=edge'>\n");
    printf("    <title>Bank &bull; Turio &bull; RuCTFE 2015</title>\n");
    printf("    <meta name='description' content='The Bank'>\n");
    printf("    <meta name='keywords' content='Turio, RuCTFE, Bank' />\n");
    printf("    <meta name='author' content='Hackerdom team, hackerdom.ru, Alexander Bersenev aka bay, bay@hackerdom.ru'>\n");
    printf("    <meta name='viewport' content='width=device-width, initial-scale=1'>\n");
    printf("    <link href='http://fonts.googleapis.com/css?family=Roboto:400,700,500' rel='stylesheet' type='text/css'>\n");
    printf("    <link href='http://fonts.googleapis.com/css?family=Lato:300,400' rel='stylesheet' type='text/css'>\n");
    printf("    <link rel='stylesheet' href='/static/inc/bootstrap/css/bootstrap.min.css'>\n");
    printf("    <link rel='stylesheet' href='/static/css/unika.min.css'>\n");
    printf("    <link rel='stylesheet' href='/static/css/turio.css'>\n");
    printf("    <link rel='stylesheet' href='/static/css/bank.css'>\n");
    printf("</head>\n");
    printf("<body class='service-page' data-spy='scroll' data-target='#main-navbar'>\n");
    printf("    <div class='page-loader'></div>  <!-- Display loading image while page loads -->\n");
    printf("    <div class='body'>\n");
    printf("        <header id='header' class='header-main'>\n");
    printf("            <nav id='main-navbar' class='navbar navbar-default navbar-fixed-top' role='navigation'>\n");
    printf("                <div class='container'>\n");
    printf("                    <div class='navbar-header'>\n");
    printf("                        <button type='button' class='navbar-toggle collapsed' data-toggle='collapse' data-target='#bs-navbar-collapse'></button>\n");
    printf("                        <a class='navbar-brand main-page-link' href='/home'>&larr;</a>\n");
    printf("                        <a class='navbar-brand' href='/'>The Bank</a>\n");
    printf("                    </div>\n");
    printf("                    <div class='collapse navbar-collapse' id='bs-navbar-collapse'>\n");
    printf("                        <ul class='nav navbar-nav navbar-right'>\n");
    printf("                            <li><a href='/'>Home</a></li>\n");
    if(show_logout) {
    printf("                            <li><a href='/'>Logout</a></li>\n");
    }
    printf("                        </ul>\n");
    printf("                    </div>\n");
    printf("                </div>\n");
    printf("            </nav>\n");
    printf("        </header>\n");
    printf("        <section class='main-block' data-stellar-background-ratio='0.5'>\n");
    printf("            <div class='container'>\n");
    printf("                <div class='caption'>\n");
    printf("                    <div class='row'>\n");
    printf("                        <div class='col-sm-2 center-block'>\n");
    printf("                            <a href='/'>\n");
    printf("                                <img src='/static/logos/bank.png' class='logo img-responsive'>\n");
    printf("                            </a>\n");
    printf("                        </div>\n");
    printf("                        <div class='col-sm-8 content-block'>\n");
    fflush(stdout);
}

void print_footer() {
    printf("                            </div>\n");
    printf("                        </div>\n");
    printf("                    </div>\n");
    printf("                </div>\n");
    printf("            </section>\n");
    printf("        </div>\n");
    printf("        <script src='/static/inc/jquery/jquery-1.11.1.min.js'></script>\n");
    printf("        <script src='/static/inc/bootstrap/js/bootstrap.min.js'></script>\n");
    printf("        <script src='/static/js/theme.js'></script>\n");
    printf("    </body>\n");
    printf("</html>\n");
    fflush(stdout);
}

void print_bank_redirect() {
    printf("<script type='text/javascript'>\n");
    printf("(function(){\n");
    printf(   "setTimeout(function(){\n");
    printf("     window.location='bank.cgi';\n");
    printf("   },2000); \n");
    printf("})();\n");
    printf("</script>\n");
    fflush(stdout);
}

void print_accounts_redirect() {
    printf("<script type='text/javascript'>\n\n");
    printf("var getUrlParameter = function getUrlParameter(sParam) {\n");
    printf("    var sPageURL = decodeURIComponent(window.location.search.substring(1)),\n");
    printf("        sURLVariables = sPageURL.split('&'),\n");
    printf("        sParameterName, i;\n");
    printf("    for (i = 0; i < sURLVariables.length; i++) {\n");
    printf("        sParameterName = sURLVariables[i].split('=');\n");
    printf("        if (sParameterName[0] === sParam) {\n");
    printf("            return sParameterName[1] === undefined ? true : sParameterName[1];\n");
    printf("        }\n");
    printf("    }\n");
    printf("};\n");
    printf("(function(){\n");
    printf(   "setTimeout(function(){\n");
    printf("     window.location='account.cgi?login=' + getUrlParameter('login');\n");
    printf("   },2000); \n");
    printf("})();\n");
    printf("</script>\n");
    fflush(stdout);
}