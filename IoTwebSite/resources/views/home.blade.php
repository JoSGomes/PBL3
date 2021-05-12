@extends('layouts.template')

@section('body')
    <div class="container">
        <div class="row">
            <div class="col">
                <div class="subtitle ">Dispositivo [ESTADO]</div>
                <div class="row position-relative top-50">
                    <input type="checkbox" class="btn-check" id="btn-check-outlined" autocomplete="off">
                    <label class="btn btn-outline-light" for="btn-check-outlined"> Alarme </label>
                </div>
            </div>

            <div class="col">
                <h1 class="subtitle">Contatos de emergência</h1>
                <div class="scrollable position-relative top-50">
                    <table class="table table-hover table-bordeless">
                        <thead>
                        <tr>
                            <th scope="col">Nome</th>
                            <th scope="col">Número</th>
                        </tr>
                        </thead>
                        <tbody>
                        <tr >
                            <td>Mark</td>
                            <td>Otto</td>
                        </tr>
                        </tbody>
                    </table>
                </div>

            </div>
        </div>
    </div>
@endsection


